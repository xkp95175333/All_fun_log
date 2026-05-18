#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <stdint.h>
#include <algorithm>

std::vector<uint8_t> ReadBlock(uint64_t address,size_t size)
{
    std::vector<uint8_t> out(size);

    if(!address||!size)
        return out;

    driver.readmnew(
        address,
        out.data(),
        (uint32_t)size
    );

    return out;
}

uint64_t ReadChain(
uint64_t base,
const std::vector<uint64_t>& offsets)
{
    if(offsets.empty())
        return 0;

    uint64_t result=
    driver.read<uint64_t>(
    base+offsets[0]);

    for(size_t i=1;
        i<offsets.size();
        i++)
    {
        if(!result)
            return 0;

        result=
        driver.read<uint64_t>(
        result+offsets[i]
        );
    }

    return result;
}

namespace setupimguidebug
{
    bool AutoupdateUorld=false;

    uint64_t baseaddress=0;

    size_t moduleSize=0;

    uint32_t base_Size=0;

    uint64_t endAddress=0;

    std::vector<unsigned char>
    memoryData;
}


namespace AobData
{

struct AOBResult
{
    uint64_t Address=0;

    uint64_t Offset=0;

    std::string Pattern;

    std::vector<uint8_t>
    Bytes;

    bool Valid() const
    {
        return Address!=0;
    }

    uint64_t Rip() const
    {
        if(Bytes.size()<7)
            return 0;

        int32_t rel=
        *(int32_t*)
        (&Bytes[3]);

        return
        Address+
        7+
        rel;
    }

    uint64_t ReadPtr()
    {
        auto ptr=Rip();

        if(!ptr)
            return 0;

        return
        driver.read<uint64_t>(
        ptr
        );
    }

    template<class T>
    T Read(size_t off=0)
    {
        return
        driver.read<T>(
        Address+off
        );
    }

    void Dump()
    {
        printf(
        "\nAOB=%s\n",
        Pattern.c_str());

        printf(
        "Address=0x%llX\n",
        Address);

        printf(
        "Offset=0x%llX\n",
        Offset);

        printf(
        "Rip=0x%llX\n",
        Rip());

        printf(
        "Ptr=0x%llX\n",
        ReadPtr());

        printf("Bytes:\n");

        for(auto x:Bytes)
        {
            printf(
            "%02X ",
            x);
        }

        printf("\n");
    }
};



class AOBScanner
{
public:

AOBResult Find(
const std::string& aob,
size_t saveBytes=64)
{
    AOBResult result;

    result.Pattern=aob;

    std::vector<uint8_t>
    pattern;

    std::vector<bool>
    mask;

    std::istringstream
    iss(aob);

    std::string s;

    while(iss>>s)
    {
        if(
        s=="?"
        ||
        s=="??")
        {
            pattern.push_back(
            0);

            mask.push_back(
            false);
        }
        else
        {
            pattern.push_back(
            (uint8_t)
            strtoul(
            s.c_str(),
            0,
            16));

            mask.push_back(
            true);
        }
    }

    auto& mem=
    setupimguidebug::
    memoryData;

    if(
    mem.size()<
    pattern.size())
        return result;

    size_t last=
    mem.size()
    -
    pattern.size();

    for(size_t i=0;
        i<=last;
        i++)
    {
        bool ok=true;

        for(size_t j=0;
            j<pattern.size();
            j++)
        {
            if(
            mask[j]
            &&
            mem[i+j]
            !=pattern[j]
            )
            {
                ok=false;
                break;
            }
        }

        if(ok)
        {
            result.Offset=i;

            result.Address=
            setupimguidebug::
            baseaddress+i;

            size_t sz=
            min(
            saveBytes,
            mem.size()-i
            );

            result.Bytes.assign(
            mem.begin()+i,
            mem.begin()+i+sz
            );

            return result;
        }
    }

    return result;
}

};

inline AOBScanner Scan;


/* เก็บผลไว้ใช้หลัง clear() */

inline AOBResult World;

inline AOBResult Camera;

inline AOBResult Replay;

inline AOBResult Bone;

inline AOBResult W2S;

}

static const char AobWorld[]=
"48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9";

static const char AobCamera[]=
"48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB";

AobData::World=
AobData::Scan.Find(
AobWorld
);

AobData::Camera=
AobData::Scan.Find(
AobCamera
);

AobData::World.Dump();

AobData::Camera.Dump();

printf(
"UWORLD=0x%llX\n",
AobData::World.ReadPtr()
);

printf(
"CAMERA=0x%llX\n",
AobData::Camera.ReadPtr()
);


setupimguidebug::
memoryData.clear();

//encupdate
AobData::World.Address

AobData::World.Offset

AobData::World.Bytes

AobData::World.Rip()

AobData::World.ReadPtr()

  auto uworld=
AobData::World.ReadPtr();

auto cam=
AobData::Camera.ReadPtr();
