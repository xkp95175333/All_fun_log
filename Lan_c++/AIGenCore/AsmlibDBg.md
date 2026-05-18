```cpp

ถ้าจะต่อให้ครบสายงานที่นายทำ (อ่าน PE → dump module → scan AOB → แปลงกลับ runtime address) ฉันรวมโครงไว้ให้แบบใช้กับ memoryData ที่นายโหลดมาแล้ว

เริ่มจาก struct ก่อน:

struct SectionInfo
{
    std::string name;

    uintptr_t start;
    uintptr_t end;

    uint32_t rva;
    uint32_t size;
};

std::vector<SectionInfo> PESections;

ฟังก์ชันอ่าน PE + Sections:

bool GetPEInfo()
{
    IMAGE_DOS_HEADER dos =
        driver.read<IMAGE_DOS_HEADER>(
            driver.base_address
        );

    if(dos.e_magic!=IMAGE_DOS_SIGNATURE)
        return false;

    IMAGE_NT_HEADERS nt =
        driver.read<IMAGE_NT_HEADERS>(
            driver.base_address+
            dos.e_lfanew
        );

    if(!nt.OptionalHeader.SizeOfImage)
        return false;

    setupimguidebug::baseaddress=
        driver.base_address;

    setupimguidebug::moduleSize=
        nt.OptionalHeader.SizeOfImage;

    setupimguidebug::endAddress=
        setupimguidebug::baseaddress+
        setupimguidebug::moduleSize;

    PESections.clear();

    uintptr_t secAddr=

    driver.base_address+
    dos.e_lfanew+
    sizeof(IMAGE_NT_HEADERS);

    auto secCount=
    nt.FileHeader.NumberOfSections;

    std::vector<IMAGE_SECTION_HEADER>
    sections(secCount);

    driver.readmnew(
        secAddr,
        sections.data(),
        secCount*
        sizeof(
          IMAGE_SECTION_HEADER
        )
    );

    for(auto&s:sections)
    {
        char name[9]{};

        memcpy(
            name,
            s.Name,
            8
        );

        uintptr_t start=
            driver.base_address+
            s.VirtualAddress;

        uint32_t size=
            s.Misc.VirtualSize;

        PESections.push_back(
        {
            name,

            start,

            start+size,

            s.VirtualAddress,

            size
        });

        printf(
        "%s\n"
        "Start:%p\n"
        "End:%p\n"
        "RVA:%X\n"
        "Size:%X\n\n",

        name,

        (void*)start,

        (void*)(start+size),

        s.VirtualAddress,

        size
        );
    }

    return true;
}

ตัวอย่าง:

.text
Start:7FF712341000
End:7FF712523000
RVA:1000
Size:1E2000

.pdata
Start:7FF7126AA000
End:7FF7126C0000
RVA:3AA000
Size:16000


---

ต่อ AOB scanner

แปลง:

48 8B 05 ? ? ? ? 48 8B

เป็น:

bytes={48,8B,05,0,0,0,0,48,8B}

mask=
xxx????xx

Parser:

void ParsePattern(
    const char* sig,

    std::vector<int>&bytes,

    std::string&mask)
{
    while(*sig)
    {
        if(*sig==' ')
        {
            sig++;
            continue;
        }

        if(*sig=='?')
        {
            bytes.push_back(0);

            mask+='?';

            if(*(sig+1)=='?')
                sig++;

            sig++;

            continue;
        }

        char b[3]=
        {
            sig[0],
            sig[1],
            0
        };

        bytes.push_back(
        strtoul(
            b,
            nullptr,
            16
        ));

        mask+='x';

        sig+=2;
    }
}

scanner:

uintptr_t ScanAOB(
    const char* pattern,

    uintptr_t begin,

    uint32_t size)
{
    std::vector<int> bytes;

    std::string mask;

    ParsePattern(
        pattern,
        bytes,
        mask
    );

    uintptr_t base=
        setupimguidebug::baseaddress;

    auto* mem=
    setupimguidebug::memoryData.data();

    size_t start=
        begin-base;

    size_t end=
        start+size;

    for(
        size_t i=start;
        i<end-
        bytes.size();
        i++
    )
    {
        bool found=true;

        for(
            size_t j=0;
            j<bytes.size();
            j++
        )
        {
            if(
               mask[j]=='x'&&
               mem[i+j]!=
               bytes[j]
            )
            {
                found=false;
                break;
            }
        }

        if(found)
        {
            return
            base+i;
        }
    }

    return 0;
}

ใช้แสกนทั้ง module:

auto addr=
ScanAOB(
"48 8B 05 ? ? ? ? 48 8B",
setupimguidebug::baseaddress,
setupimguidebug::moduleSize
);

หรือเฉพาะ .text

for(auto&s:PESections)
{
    if(s.name==".text")
    {
        auto a=
        ScanAOB(
        "48 8B 05 ? ? ? ?",
        s.start,
        s.size
        );

        printf(
        "Found:%p\n",
        (void*)a
        );
    }
}

จากนั้นต่อเข้าฟังก์ชัน Capstone ที่คุยกัน:

auto uworld=
asmLib(
handle,
addr,
driver.base_address
);

flow จะเป็น:

GetPEInfo()

↓

โหลด memoryData

↓

ScanAOB(".text")

↓

เจอ instruction

↓

asmLib()

↓

คำนวณ RIP

↓

ได้ UWorld

อันนี้เป็น flow ที่ SDK dumpers / UE scanners ใช้กันเยอะ เพราะเร็วกว่า driver.read() ทีละ byte มาก.
//222
อันนั้นเป็น macro ของ Capstone สำหรับหา "ตำแหน่ง offset ของ instruction ใน buffer ที่ disasm มา"

โค้ด:

#define CS_INSN_OFFSET(insns, post) \
(insns[post - 1].address - insns[0].address)

เขียนแบบฟังก์ชันจะประมาณนี้:

size_t GetInsnOffset(
    cs_insn* insns,
    int position)
{
    return
    insns[position-1].address -
    insns[0].address;
}

คำว่า post มันใช้ position (เริ่ม 1) ไม่ใช่ index

ตัวอย่าง:

สมมุติอ่าน:

7FF712340000 push rbx        ;1 byte
7FF712340001 sub rsp,20      ;4 byte
7FF712340005 mov rax,[rip+123]
7FF71234000C call xxxxx

Capstone จะได้:

insns[0].address=0x7FF712340000
insns[1].address=0x7FF712340001
insns[2].address=0x7FF712340005
insns[3].address=0x7FF71234000C

ลอง:

CS_INSN_OFFSET(insns,3)

จะเป็น:

0x7FF712340005
-
0x7FF712340000

=
5

แปลว่า instruction ตัวที่ 3 อยู่ byte ที่ 5 ใน buffer

มีประโยชน์เวลา:

ReadBlock(
 address,
 200
);

แล้ว disasm หลายบรรทัด จากนั้นอยากย้อนกลับไปหา byte จริง:

auto off=
CS_INSN_OFFSET(
insn,
x
);

printf(
"%02X",
mem[off]
);


---

ส่วนเรื่องเพิ่ม baseAddr + ตั้ง RIP base เอง เข้าใจที่นายจะทำ:

แทน:

mov rax,[rip+1234]

Capstone คำนวณ:

real=
insn.address+
insn.size+
disp

แต่นายอยาก:

real=
baseAddress+
offset+
disp

เพื่อใช้ dump/local memory

จัดฟังก์ชันแบบรวมให้:

uint64_t asmLib(
    csh handle,
    uint64_t targetAddress,
    uint64_t baseAddress,
    size_t readSize=32,
    std::string* outAsm=nullptr)
{
    auto mem=
        ReadBlock(
            targetAddress,
            readSize
        );

    if(mem.empty())
        return 0;

    cs_insn* insn=nullptr;

    size_t count=
    cs_disasm(
        handle,
        mem.data(),
        mem.size(),
        targetAddress,
        1,
        &insn
    );

    if(!count)
        return 0;

    if(outAsm)
    {
        *outAsm=
        std::string(
            insn[0].mnemonic
        )+
        " "+
        insn[0].op_str;
    }

    auto& x86=
    insn[0].detail->x86;

    uint64_t Rip=0;

    for(int i=0;i<x86.op_count;i++)
    {
        auto& op=
        x86.operands[i];

        if(
          op.type==
          X86_OP_MEM &&

          op.mem.base==
          X86_REG_RIP
        )
        {
            intptr_t localOffset=
            targetAddress-
            baseAddress;

            Rip=
            baseAddress+
            localOffset+
            insn[0].size+
            op.mem.disp;

            break;
        }

        if(
            op.type==
            X86_OP_IMM
        )
        {
            Rip=
            op.imm;

            break;
        }
    }

    cs_free(
        insn,
        count
    );

    return Rip;
}

ใช้:

std::string op;

auto addr=
asmLib(
    handle,
    AddressFound,
    driver.base_address,
    32,
    &op
);

printf(
"ASM: %s\n",
op.c_str()
);

printf(
"Rip: %p\n",
(void*)addr
);

ผล:

ASM: mov rax,qword ptr [rip+0x1234]

Rip:
00007FF743210000

หรือถ้าอ่านจาก dump:

Rip=
base+
offset+
disp

ไม่ต้อง hardcode +3 +4 +7 อีกเลย

อีกอย่าง op_str มีประโยชน์มากเวลา debug AOB:

if(op.find("[rip+")!=std::string::npos)

หรือ:

if(op.find("qword ptr")!=
   std::string::npos)

ไว้คัด instruction แบบเร็ว ๆ ได้ด้วย.




```
