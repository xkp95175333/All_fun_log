



```cpp
ฉันหาไฟล์ตรง ๆ ไม่เจอจากลิงก์ที่ให้ และตอนนี้ยังเปิด raw .asm กับ CNewWorld.c เนื้อหาไม่ได้จากผลค้นหา ดังนั้นยังดึง “บรรทัด ^0x36 จริง” ให้ไม่ได้จาก repo โดยตรง

แต่จากที่คุยกันมาหลายรอบ ฉันเข้าใจ flow ที่นายต้องการ: หา FullAsm → ย้อน/เดินหน้า → เจอ XOR 36h → แยก 2 path (ทีมเรา/ทีมอื่น) แบบอัตโนมัติ

จัดเป็นตัวช่วยที่ไล่บรรทัดก่อน–หลัง FullAsm และ mark จุด 36h ให้เลย:

void TraceFullAsm36(
    csh handle,
    uint64_t fullAsmAddr,
    uint64_t baseAddr)
{
    // อ่านก่อน 0x150 หลัง 0x150
    uint64_t start =
        fullAsmAddr - 0x150;

    auto mem =
        ReadBlock(
            start,
            0x300
        );

    if(mem.empty())
        return;

    cs_insn* insn=nullptr;

    size_t count=
    cs_disasm(
        handle,
        mem.data(),
        mem.size(),
        start,
        0,
        &insn
    );

    if(!count)
        return;

    int fullIndex=-1;

    for(size_t i=0;i<count;i++)
    {
        if(insn[i].address>=fullAsmAddr)
        {
            fullIndex=(int)i;
            break;
        }
    }

    if(fullIndex==-1)
    {
        cs_free(insn,count);
        return;
    }

    printf(
    "\n===== FullASM =====\n"
    );

    // ก่อน 25 หลัง 25
    int begin=
        max(
        0,
        fullIndex-25
        );

    int end=
        min(
        (int)count-1,
        fullIndex+25
        );

    for(int i=begin;i<=end;i++)
    {
        std::string line=
        std::string(
            insn[i].mnemonic
        )+" "+
        insn[i].op_str;

        bool xor36=
            line.find("36")!=
            std::string::npos;

        bool isCenter=
            insn[i].address==
            fullAsmAddr;

        printf(
            "%s%s +%llX %p %-8s %s\n",

            isCenter?
            ">>> ":"",

            xor36?
            "[36] ":"",

            insn[i].address-
            baseAddr,

            (void*)
            insn[i].address,

            insn[i].mnemonic,

            insn[i].op_str
        );

        // ถ้าเจอ xor36
        if(xor36)
        {
            printf(
            "\n--- PATH ---\n"
            );

            // ย้อน 8 บรรทัด
            for(
            int j=max(
            begin,
            i-8
            );

            j<=min(
            end,
            i+8
            );

            j++)
            {
                printf(
                "%p %s %s\n",

                (void*)
                insn[j].address,

                insn[j].mnemonic,

                insn[j].op_str
                );
            }

            printf(
            "-------------\n"
            );
        }
    }

    cs_free(
        insn,
        count
    );
}

ใช้:

TraceFullAsm36(
    handle,
    FullAsmAddress,
    driver.base_address
);

ตัวอย่าง output:

===== FullASM =====

+245100 mov rax,qword ptr [rbx+20]
+245107 shr rax,8

[36] +24510D xor cl,36h

--- PATH ---

7FF71245100 mov rax,[rbx+20]
7FF71245107 shr rax,8
7FF7124510A movzx ecx,al
7FF7124510D xor cl,36h
7FF71245110 mov [rsp+40],cl

-------------

>>> +245130 call xxxxx

เพิ่มตัวแยก "ทีมเรา / ทีมอื่น"

หลายเกม UE มักมาก่อนด้วย:

cmp eax,1
je ally

...

cmp eax,2
je enemy

เพิ่มเช็ค:

if(
!strcmp(
insn[i].mnemonic,
"cmp"
))
{
    printf(
    "[TEAM?] %s\n",
    insn[i].op_str
    );
}

เวลา dump จะเห็นประมาณ:

cmp eax,1
je  ally

xor cl,36h

cmp eax,2
je enemy

xor dl,36h

อันนี้แหละสองทีมที่นายหา

อีกตัวช่วยแรงมากคือหา 36h จาก dump ทั้งโมดูลก่อน:

auto a=
ScanAOB(
"36",
text.start,
text.size
);

หรือ:

"80 F1 36"

เพราะหลายที xor cl,36h จะ encode เป็น:

80 F1 36

หรือ:

83 F0 36

แล้วเอา address ที่เจอโยนเข้า:

TraceFullAsm36(...)

จะไล่เส้นทางได้เร็วกว่าเปิด asm ยาวหลายพันบรรทัดเอง.

ฉันดูจากที่นายอธิบาย เป้าคือไม่ได้อยากหาแค่ AOB แต่จะไล่ “ก่อนถึง FullAsm” เพื่อหา flow ที่ทำ XOR ^0x36 และอยากไล่ให้ครบทั้ง 2 path (ฝั่งเรา/อีกฝั่ง) จาก log asm ที่ dump มา

แต่ลิงก์ raw/github ที่ให้มา ฉันเข้าถึงเนื้อหาไฟล์ตรง ๆ ไม่ได้จากผลค้นหาที่มีอยู่ตอนนี้ เลยยังชี้บรรทัด ^0x36 จริง ๆ ไม่ได้

จากโค้ดเก่าที่นายเคยแปะ:

v251.m128_u64[0] = qword_1597E4828;
v251.m128_i8[0] = HIBYTE(qword_1597E4828) ^ 0x36;
v251.m128_i8[7] ...

ตัว ^0x36 แบบนี้มักไม่ได้อยู่เดี่ยว ๆ มันมักอยู่ในสาย:

mov ...
shr ...
xor al,36h
mov ...

หรือ

movzx eax, byte ptr [...]
xor al,36h

ดังนั้นถ้าจะทำ scanner ให้หา "ก่อนถึง FullAsm" แบบอัตโนมัติ ใช้ Capstone ไล่ instruction ย้อน/เดินหน้าแทนหา AOB ตรง ๆ จะง่ายกว่า

ตัวอย่าง helper:

void ScanNearXor36(
    csh handle,
    uint64_t centerAddr,
    uint64_t baseAddr)
{
    // อ่านก่อนหน้า 0x80 และหลังอีกนิด
    uint64_t start =
        centerAddr - 0x80;

    auto mem =
        ReadBlock(
            start,
            0x120
        );

    cs_insn* insn;

    size_t count=
    cs_disasm(
        handle,
        mem.data(),
        mem.size(),
        start,
        0,
        &insn
    );

    for(size_t i=0;i<count;i++)
    {
        std::string op=
        std::string(
            insn[i].mnemonic
        )+" "+
        insn[i].op_str;

        printf(
        "+%llX %s\n",
        insn[i].address-baseAddr,
        op.c_str()
        );

        // xor ??,36h
        if(
        !strcmp(
          insn[i].mnemonic,
          "xor")
        &&
        strstr(
          insn[i].op_str,
          "36")
        )
        {
            printf(
            "\nFOUND XOR36\n"
            );

            // ย้อนก่อนหน้า 6 บรรทัด
            for(
            int j=max(
              0,
              (int)i-6
            );

            j<=min(
              (int)count-1,
              (int)i+6
            );

            j++)
            {
                printf(
                "%p %s %s\n",

                (void*)
                insn[j].address,

                insn[j].mnemonic,

                insn[j].op_str
                );
            }
        }
    }

    cs_free(
      insn,
      count
    );
}

ใช้:

ScanNearXor36(
    handle,
    FullAsmAddr,
    driver.base_address
);

จะได้ประมาณ:

+245100 mov rax,qword ptr [rbx+20]
+245107 shr rax,8
+24510A movzx ecx,al
+24510D xor cl,36h
+245110 mov [rsp+40],cl

แล้วนายจะเห็น "ทีมฝั่งเรา" กับ "อีกฝั่ง" เอง เพราะมันมักเป็นคนละ branch:

cmp teamid,1
je ally

xor cl,36h
...
jmp out

ally:
xor cl,36h
...

หรือคนละ source register:

mov al,[rbx+30]
xor al,36h

mov dl,[rsi+30]
xor dl,36h

ถ้านายอัปโหลด asmNewWorld.asm ตรง ๆ ในแชต (หรือวางช่วงรอบ 0x36 สัก ~100 บรรทัด) ฉันไล่ flow ให้ต่อได้เลยว่า "บวกลบก่อนถึง FullAsm" ต้องเดินกี่บรรทัดและสองทีมแยกตรงไหน เพราะตอนนี้ไม่มีเนื้อไฟล์จริงให้อ่าน.




```
