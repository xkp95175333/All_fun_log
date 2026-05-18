
# Offset / Runtime Address Flow

สรุป flow ตั้งแต่ PE → Dump → ScanAOB → ASM → RIP → Runtime

## Flow รวม

```text
GetPE
 ↓
Dump Module
 ↓
memoryData
 ↓
Parse Sections(.text/.pdata)
 ↓
ScanAOB
 ↓
Address
 ↓
ReadBlock
 ↓
Capstone
 ↓
RIP Resolve
 ↓
Runtime Address
 ↓
FullASM
 ↓
XOR36
 ↓
Team Path
```

## 1) PE Header

อ่าน:

```cpp
IMAGE_DOS_HEADER
IMAGE_NT_HEADERS
IMAGE_SECTION_HEADER
```

ได้:

```cpp
baseAddress
moduleSize
endAddress
```

สูตร:

```cpp
endAddress = baseAddress + moduleSize;
```

---

## 2) memoryData Mapping

```cpp
memoryData[0]     = base+0
memoryData[0x100] = base+0x100
```

แปลง:

```cpp
Runtime = baseAddress + offset;
offset  = Runtime - baseAddress;
```

---

## 3) Sections

ตัวอย่าง:

```text
.text
.rdata
.data
.pdata
```

ใช้ scan เฉพาะ:

```cpp
ScanAOB(pattern,text.start,text.size);
```

---

## 4) ScanAOB

ตัวอย่าง:

```cpp
48 8B 05 ? ? ? ?
```

ผล:

```text
7FF712451000
```

---

## 5) ReadBlock

```cpp
auto mem =
ReadBlock(
 address,
 32
);
```

ส่งต่อ:

```cpp
cs_disasm()
```

---

## 6) RIP Relative

```asm
48 8B 05 xx xx xx xx
```

สูตร:

```cpp
real =
insn.address +
insn.size +
disp;
```

---

## 7) Capstone Operand

```cpp
if(
 op.type==X86_OP_MEM &&
 op.mem.base==X86_REG_RIP
)
{
 return
 insn.address+
 insn.size+
 op.mem.disp;
}
```

---

## 8) CS_INSN_OFFSET

```cpp
CS_INSN_OFFSET(insn,3)
```

คือ:

```cpp
insn[position-1].address
-
insn[0].address
```

ใช้ย้อนหา:

```cpp
mem[offset]
```

---

## 9) FullASM → XOR36

```text
AOB
↓
FullASM
↓
ย้อนก่อนหน้า
↓
xor ?,36h
↓
cmp/team
↓
ally/enemy
```

ค้นหา:

```cpp
80 F1 36
83 F0 36
```
