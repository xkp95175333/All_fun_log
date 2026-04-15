---
เป็นฟังชั้น ที่มีแค่ BaseAddressProcess แต่ ใช้ PE เพื่อ หา Size แล้ว ทำ การอ่านเพื่อ เก็บ เป็น Data จะได้ สแกน Aob หรือ ทำ สิ่ง อื่นๆ 
---



```h


// ฟังก์ชันเปรียบเทียบไบต์
bool ComparePattern(const BYTE* data, const BYTE* pattern, const char* mask) {
	while (*mask) {
		if (*mask == 'x' && *data != *pattern) {
			return false;
		}
		++mask, ++data, ++pattern;
	}
	return true;
}
 
uint64_t SearchAOBUpdate(std::vector<BYTE> memoryData,const std::string aobPattern, uint64_t baseaddress) {
	std::vector<uint8_t> pattern;
	std::vector<bool> mask;

	// แปลง AOB จาก String เป็น Byte และ Mask
	std::istringstream iss(aobPattern);
	std::string byteStr;
	while (iss >> byteStr) {
		if (byteStr == "??" || byteStr == "?") {
			pattern.push_back(0x00);
			mask.push_back(false);
		}
		else {
			pattern.push_back(static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16)));
			mask.push_back(true);
		}
	}

	// ค้นหาใน Memory  โมดูลหลัก 
	size_t patternSize = pattern.size();
	for (size_t i = 0; i < memoryData.size() - patternSize; ++i)
	{
		bool found = true;
		for (size_t j = 0; j < patternSize; ++j) {
			if (mask[j] && memoryData[i + j] != pattern[j]) {
				found = false;
				break;
			}
		}
		if (found) {

			return baseaddress + i;

		}
	}

}
uint64_t GetRipRelative(uint8_t* instr)
{
    int32_t offset = *(int32_t*)(instr + 3);
    return (uint64_t)(instr + 7 + offset);
}

uint64_t CalculateUworldAddress(uint64_t Addr, uint64_t base) {
	if (Addr == 0) return 0;
	uint64_t addrtoUwrold = Addr + 3;
	uint32_t relativeOffset = driver.read<uint32_t>(addrtoUwrold);
	if (relativeOffset == 0) return 0;
	return addrtoUwrold + 4 + relativeOffset;
}





namespace UpdateAob
{
	bool AutoupdateUorld = false;
	uint64_t baseaddress = NULL;
	uint32_t  base_Size = 0;
	size_t moduleSize = 0;
	uint64_t endAddress = NULL; 
	std::vector<BYTE> memoryData;

	uint32_t Uworld = 0x16A1D5B8;
	uint32_t Gname = 0x173DB7C0;



}
 
void GetdataUpdate() {
	IMAGE_DOS_HEADER dosHeader = driver.read<IMAGE_DOS_HEADER>(driver.base_address);
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		printfEx(_("Invalid DOS Header signature!\n"));
		return;
	}

	IMAGE_NT_HEADERS ntHeaders = driver.read<IMAGE_NT_HEADERS>(driver.base_address + dosHeader.e_lfanew);
	if (ntHeaders.OptionalHeader.SizeOfImage == 0) {
		printfEx(_("Failed to read module size!\n"));
		return;
	}

	UpdateAob::base_Size = ntHeaders.OptionalHeader.SizeOfImage;
	printf(_("driver.base_Size: %08X\n"), UpdateAob::base_Size);

	// อ่านทีละ Block แทนการโหลดทั้งหมด
	UpdateAob::moduleSize = UpdateAob::base_Size;
	UpdateAob::baseaddress = driver.base_address;
	UpdateAob::endAddress = UpdateAob::baseaddress + UpdateAob::moduleSize - 1; // Address สุดท้ายของโมดูล


	UpdateAob::memoryData.resize(UpdateAob::moduleSize);// std::vector<BYTE> memoryData;

	size_t totalRead = 0;
	unsigned char buffer[4096] = { 0 };
	for (size_t i = 0; i < UpdateAob::moduleSize; i += sizeof(buffer)) {
		size_t readSize = min(sizeof(buffer), UpdateAob::moduleSize - i);
		driver.readmemory(((PVOID)(UpdateAob::baseaddress + i)), &buffer, readSize); // ฟังชั้น อ่าน 
		std::copy(buffer, buffer + readSize, UpdateAob::memoryData.begin() + i);
		totalRead += readSize;

	}
	 
	// ค้นหา AOB 
	static const char AddrUworld[] = "48 8B 35 ? ? ? ? EB 31";
	auto value = SearchAOBUpdate(UpdateAob::memoryData,AddrUworld, UpdateAob::baseaddress);
	printf(_("AddrUworld 0x%llX\n"), value);

	auto UworldAddress = CalculateUworldAddress(value, driver.base_address);
	printf(_("Calculated Uworld Address: 0x%llX\n"), UworldAddress);

	// อ่านค่า UWorld
	auto addrUworld = driver.read<uintptr_t>(UworldAddress);
	printf(_("addrUworld: 0x%llX\n"), addrUworld);

	UpdateAob::Uworld = UworldAddress - driver.base_address;
	printf(_("offset::Uworld: 0x%llX\n"), UpdateAob::Uworld);

	// ค้นหา Gname
	static const char AddrGname[] = "48 8D 35 ? ? ? ? EB 16"; 
	auto Gnamevalue = SearchAOBUpdate(UpdateAob::memoryData, AddrGname, UpdateAob::baseaddress);
	printf(_("Gnamevalue 0x%llX\n"), Gnamevalue);

	auto addrGname = CalculateUworldAddress(Gnamevalue, driver.base_address);
	printf(_("addrGname: 0x%llX\n"), addrGname);
	UpdateAob::Gname = addrGname - driver.base_address;
	printf(_("offset::Gname: 0x%llX\n"), UpdateAob::Gname);
	 
	UpdateAob::memoryData.clear();
	UpdateAob::AutoupdateUorld = false;

}

```

---
new aiGen
---
```cpp

struct AOBResult
{
    bool found = false;

    uint64_t instrAddr = 0;   // จุดเจอ AOB (.text)
    uint64_t ripAddr = 0;     // instr + 3
    int32_t  relOffset = 0;   // displacement
    uint64_t targetAddr = 0;  // address จริง (GNames/UWorld)
    uint64_t finalOffset = 0; // offset = target - base
};
AOBResult SearchAOBEx(
    const std::vector<BYTE>& memoryData,
    const std::string& aobPattern,
    uint64_t baseaddress)
{
    AOBResult result;

    std::vector<uint8_t> pattern;
    std::vector<bool> mask;

    std::istringstream iss(aobPattern);
    std::string byteStr;

    while (iss >> byteStr)
    {
        if (byteStr == "??" || byteStr == "?")
        {
            pattern.push_back(0x00);
            mask.push_back(false);
        }
        else
        {
            pattern.push_back((uint8_t)std::stoi(byteStr, nullptr, 16));
            mask.push_back(true);
        }
    }

    size_t patternSize = pattern.size();

    for (size_t i = 0; i < memoryData.size() - patternSize; ++i)
    {
        bool found = true;

        for (size_t j = 0; j < patternSize; ++j)
        {
            if (mask[j] && memoryData[i + j] != pattern[j])
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            result.found = true;

            // 📍 จุดเจอ instruction
            result.instrAddr = baseaddress + i;

            // 📍 RIP-relative decode
            result.ripAddr = result.instrAddr + 3;
            result.relOffset = *(int32_t*)(&memoryData[i + 3]);

            result.targetAddr = result.instrAddr + 7 + result.relOffset;
            result.finalOffset = result.targetAddr - baseaddress;

            return result;
        }
    }

    return result;
}
//UWorld
auto resU = SearchAOBEx(UpdateAob::memoryData, AddrUworld, UpdateAob::baseaddress);

if (resU.found)
{
    printf("UWorld instr: 0x%llX\n", resU.instrAddr);
    printf("UWorld target: 0x%llX\n", resU.targetAddr);
    printf("UWorld offset: 0x%llX\n", resU.finalOffset);

    auto addrUworld = driver.read<uintptr_t>(resU.targetAddr);

    UpdateAob::Uworld = resU.finalOffset;
}
//GNames

auto resG = SearchAOBEx(UpdateAob::memoryData, AddrGname, UpdateAob::baseaddress);

if (resG.found)
{
    printf("GNames instr: 0x%llX\n", resG.instrAddr);
    printf("GNames target: 0x%llX\n", resG.targetAddr);
    printf("GNames offset: 0x%llX\n", resG.finalOffset);

    UpdateAob::Gname = resG.finalOffset;
}



```
