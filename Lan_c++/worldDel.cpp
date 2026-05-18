```md
โอเค จาก C ที่นายแปะ ตอนนี้ลำดับจริงน่าจะเป็น:

if (off_1597E1820())
{
    x=qword_1597E4828;

    byte[0]=HIBYTE(x)^0x36;
    byte[7]=(x&0xFF)^0x36;

    v109=x;
}
else
{
    v109=qword_1597E2828;
}

ดังนั้นสิ่งที่ต้องส่งไม่ใช่ UWorld ตรง ๆ แต่ต้องมี:

1. address ของ qword_1597E4828


2. address ของ qword_1597E2828


3. function off_1597E1820() ถ้าจะเช็ค branch จริง



แต่ถ้าเราไม่ได้ execute function ในเกม ให้ทำแบบ fallback:

อ่านสองค่า

ถ้า decrypt แล้ว pointer สมเหตุสมผลใช้ตัวนั้น

ไม่งั้นใช้อีกตัว


รวมเข้ากับ .h เดิมแบบวางทับเพิ่มได้เลย:

namespace WorldRuntime
{
    inline uint64_t PtrA=0; // qword_1597E4828
    inline uint64_t PtrB=0; // qword_1597E2828

    union XMMFix
    {
        __m128i xmm;
        uint8_t b[16];
        uint64_t q[2];
    };

    inline uint64_t FixPtr(uint64_t x)
    {
        XMMFix v{};

        v.q[0]=x;

        // HIBYTE(qword)^0x36

        v.b[0]=
        ((x>>8)&0xFF)^0x36;

        // low byte ^0x36

        v.b[7]=
        (x&0xFF)^0x36;

        return v.q[0];
    }

    inline bool IsValidPtr(
    uint64_t x)
    {
        if(!x)
            return false;

        if(x<0x100000)
            return false;

        if(x>0x7FFFFFFFFFFF)
            return false;

        return true;
    }

    inline uint64_t Get()
    {
        uint64_t x1=
        driver.read<uint64_t>(
        PtrA
        );

        uint64_t x2=
        driver.read<uint64_t>(
        PtrB
        );

        auto decrypt=
        FixPtr(x1);

        if(
        IsValidPtr(
        decrypt
        ))
        {
            return decrypt;
        }

        return x2;
    }
}

แล้วใน GetdataOffset() เพิ่ม:

static const char AobA[]=
"48 8B 05 ? ? ? ?";

auto A=
AobData::Scan.Find(
AobA
);

if(A.Valid())
{
    WorldRuntime::PtrA=
    A.Rip();

    printf(
    "PtrA=0x%llX\n",
    WorldRuntime::PtrA
    );
}

ส่วนตัวที่สอง:

auto B=
AobData::Scan.Find(
"48 8B 0D ? ? ? ?"
);

if(B.Valid())
{
    WorldRuntime::PtrB=
    B.Rip();

    printf(
    "PtrB=0x%llX\n",
    WorldRuntime::PtrB
    );
}

ใช้งาน:

uint64_t uworld=
WorldRuntime::Get();

printf(
"UWorld=0x%llX\n",
uworld
);

สิ่งที่จะเกิด:

รอบแรก:

driver.read<uint64_t>(
qword_1597E4828
)

ได้:

0x123456789ABCDEF0

เข้า:

FixPtr()

แก้:

byte0
byte7

คืน pointer ใหม่

ถ้า pointer ดูจริง:

return decrypt;

ไม่งั้น:

return qword_1597E2828

ดังนั้นนายต้องเก็บแค่:

WorldRuntime::PtrA
WorldRuntime::PtrB

ไม่ต้องส่ง __m128i เอง เพราะฟังก์ชันสร้าง union ให้แล้ว

และไม่ต้องสน +7 ใน F5 เพราะ A.Rip() คำนวณให้แล้วจาก raw asm.
ฉันเห็นสิ่งที่นายกำลังทำแล้ว: พยายามไล่ logic จาก ASM/IDA เพื่อดึง pointer (UWorld) จาก qword_*, XMM และ XOR ที่เปลี่ยนตาม runtime แต่จุดที่ต้องระวังคือ F5 หลอกตาได้ง่ายมาก — ถ้ารีบ hardcode xor 0x36 หรือแก้ byte ตายตัว อาจใช้ได้แค่ build เดียวแล้วพังรอบหน้า

จากโค้ดนี้:

v251.m128_u64[0] = qword_1597E4828;
v251.m128_i8[0] = HIBYTE(qword_1597E4828) ^ 0x36;
v251.m128_i8[7] = qword_1597E4828 ^ 0x36;
v109 = v251.m128_u64[0];

สิ่งที่มั่นใจได้คือ:

โหลดค่า qword_1597E4828

แก้ byte [0] และ [7]

XOR 0x36

เอา m128_u64[0] กลับมา


แต่ยัง ไม่ควรสรุปว่าเป็น decrypt ทั้งระบบ จนกว่าจะดู raw asm รอบนั้น เพราะ F5 ชอบยุบ pinsrb, movq, movdqa แล้วทำให้ดูเป็น union ธรรมดา

สิ่งที่ควรเพิ่มคือ validate pointer หลายชั้น แทนการเชื่อค่า decrypt ตรง ๆ:

namespace WorldRuntime
{
    union XMMFix
    {
        __m128i xmm;
        uint8_t b[16];
        uint64_t q[2];
    };

    inline uint64_t PtrA=0;
    inline uint64_t PtrB=0;

    inline bool IsCanonical(uint64_t p)
    {
        if(!p) return false;

        // usermode x64 canonical range
        if(p < 0x100000)
            return false;

        if(p > 0x00007FFFFFFFFFFF)
            return false;

        return true;
    }

    inline bool IsReadable(uint64_t p)
    {
        if(!IsCanonical(p))
            return false;

        uint64_t t=
        driver.read<uint64_t>(p);

        return t!=0;
    }

    inline uint64_t TryXor36(uint64_t x)
    {
        XMMFix v{};

        v.q[0]=x;

        v.b[0]=
        ((x>>8)&0xFF)^0x36;

        v.b[7]=
        (x&0xFF)^0x36;

        return v.q[0];
    }

    inline uint64_t TryByteSwap(uint64_t x)
    {
        XMMFix v{};

        v.q[0]=x;

        std::swap(
        v.b[0],
        v.b[7]);

        return v.q[0];
    }

    inline uint64_t TryRotate(uint64_t x)
    {
        return
        _rotr64(x,8);
    }

    inline uint64_t Resolve()
    {
        uint64_t a=
        driver.read<uint64_t>(
        PtrA
        );

        uint64_t b=
        driver.read<uint64_t>(
        PtrB
        );

        uint64_t candidate[5]=
        {
            a,
            TryXor36(a),
            TryByteSwap(a),
            TryRotate(a),
            b
        };

        for(auto p:candidate)
        {
            if(!IsReadable(p))
                continue;

            auto test=
            driver.read<uint64_t>(p);

            if(test &&
              IsCanonical(test))
            {
                printf(
                "valid=0x%llX\n",
                p);

                return p;
            }
        }

        return 0;
    }
};

ใช้:

auto world=
WorldRuntime::Resolve();

printf(
"UWorld=0x%llX\n",
world
);

แนวคิดคือ:

1. อ่าน PtrA


2. ลองหลายรูปแบบ:

ดิบ

XOR byte แบบ F5

swap byte

rotate

fallback PtrB



3. เช็คว่า pointer:

อยู่ใน canonical range

อ่านได้

pointer ที่ชี้ต่อดูสมเหตุสมผล




เพราะตอนนี้ยังไม่มี raw asm เต็มของ off_1597E1820() จึงยังไม่รู้ว่ามี rol, bswap, xor reg, หรือ key runtime ซ่อนอยู่หรือไม่ การทำ validation chain จะทนกว่า hardcode decrypt แบบเดียวมากกว่า.
  
```
