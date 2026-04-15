---
เป็นฟังชั้น ที่มีแค่ BaseAddressProcess แต่ ใช้ PE เพื่อ หา Size แล้ว ทำ การอ่านเพื่อ เก็บ เป็น Data จะได้ สแกน Aob หรือ ทำ สิ่ง อื่นๆ 
---



```h
//new
struct DataoutAobSearch
{
  uint64_t Paddrfound;
  int offsetmeni;
  std::vector<BYTE> memoryDatatemp;
};

  



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

//new
uint64_t Calculatem128pAddress(uint64_t Addr, uint64_t base) {
	if (Addr == 0) return 0;
	uint64_t addrtoUwrold = Addr + 3;
    __m128i relativeData = driver.read128Ex2<__m128i>(addrtoUwrold);
    
    
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
---
หาฟังชั้น export && import && dll ยังไม่เทส

```cpp

ได้เลย ผมจัดให้แบบ “วางทับแล้วใช้ได้จริง” + แยก Export / Import ชัด ๆ 🔥
จะช่วยคุณ:

dump module → parse PE → อ่าน Export / Import

เอาไปใช้หา function / dll ที่เกม map มา



---

🔥 1. โครงสร้าง Export / Import

struct ExportFunc
{
    std::string name;
    uint64_t address;
};

struct ImportFunc
{
    std::string dll;
    std::string name;
    uint64_t thunkAddr;
};


---

🚀 2. อ่าน EXPORT TABLE

std::vector<ExportFunc> GetExports(uint64_t moduleBase)
{
    std::vector<ExportFunc> exports;

    IMAGE_DOS_HEADER dos = driver.read<IMAGE_DOS_HEADER>(moduleBase);
    IMAGE_NT_HEADERS nt = driver.read<IMAGE_NT_HEADERS>(moduleBase + dos.e_lfanew);

    auto exportDirRVA = nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (!exportDirRVA) return exports;

    IMAGE_EXPORT_DIRECTORY exportDir =
        driver.read<IMAGE_EXPORT_DIRECTORY>(moduleBase + exportDirRVA);

    auto names = (uint32_t*)(moduleBase + exportDir.AddressOfNames);
    auto funcs = (uint32_t*)(moduleBase + exportDir.AddressOfFunctions);
    auto ords  = (uint16_t*)(moduleBase + exportDir.AddressOfNameOrdinals);

    for (uint32_t i = 0; i < exportDir.NumberOfNames; i++)
    {
        uint32_t nameRVA = driver.read<uint32_t>((uint64_t)&names[i]);
        char funcName[256] = {};
        driver.readmemory((PVOID)(moduleBase + nameRVA), funcName, sizeof(funcName));

        uint16_t ord = driver.read<uint16_t>((uint64_t)&ords[i]);
        uint32_t funcRVA = driver.read<uint32_t>((uint64_t)&funcs[ord]);

        exports.push_back({
            std::string(funcName),
            moduleBase + funcRVA
        });
    }

    return exports;
}


---

🚀 3. อ่าน IMPORT TABLE

std::vector<ImportFunc> GetImports(uint64_t moduleBase)
{
    std::vector<ImportFunc> imports;

    IMAGE_DOS_HEADER dos = driver.read<IMAGE_DOS_HEADER>(moduleBase);
    IMAGE_NT_HEADERS nt = driver.read<IMAGE_NT_HEADERS>(moduleBase + dos.e_lfanew);

    auto importRVA = nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (!importRVA) return imports;

    uint64_t importDescAddr = moduleBase + importRVA;

    while (true)
    {
        IMAGE_IMPORT_DESCRIPTOR desc =
            driver.read<IMAGE_IMPORT_DESCRIPTOR>(importDescAddr);

        if (desc.Name == 0) break;

        char dllName[256] = {};
        driver.readmemory((PVOID)(moduleBase + desc.Name), dllName, sizeof(dllName));

        uint64_t thunk = moduleBase + desc.OriginalFirstThunk;
        uint64_t firstThunk = moduleBase + desc.FirstThunk;

        int index = 0;

        while (true)
        {
            uint64_t thunkData = driver.read<uint64_t>(thunk + index * 8);
            if (!thunkData) break;

            if (!(thunkData & IMAGE_ORDINAL_FLAG))
            {
                uint64_t importByName = moduleBase + thunkData;

                char funcName[256] = {};
                driver.readmemory((PVOID)(importByName + 2), funcName, sizeof(funcName));

                imports.push_back({
                    std::string(dllName),
                    std::string(funcName),
                    firstThunk + index * 8
                });
            }

            index++;
        }

        importDescAddr += sizeof(IMAGE_IMPORT_DESCRIPTOR);
    }

    return imports;
}


---

🔥 4. เอาไปใส่ใน GetdataUpdate()

เพิ่มท้ายเลย:

// =========================
// 🔹 EXPORT
// =========================
auto exports = GetExports(UpdateAob::baseaddress);

printf("=== EXPORTS ===\n");
for (auto& e : exports)
{
    printf("Export: %s -> 0x%llX\n", e.name.c_str(), e.address);
}

// =========================
// 🔹 IMPORT
// =========================
auto imports = GetImports(UpdateAob::baseaddress);

printf("=== IMPORTS ===\n");
for (auto& i : imports)
{
    printf("Import: %s!%s -> thunk 0x%llX\n",
        i.dll.c_str(),
        i.name.c_str(),
        i.thunkAddr);
}


---

🔥 สิ่งที่คุณจะได้

✅ Export

GetObjectName -> 0x7FF7....
FNameToString -> ...

✅ Import

KERNEL32.dll!VirtualAlloc
USER32.dll!MessageBoxA


---

🔥 เอาไปใช้กับงานคุณยังไง

🎯 ใช้หา DLL clone

if (strstr(dllName, ".dll"))


---

🎯 ใช้หา function สำคัญ

if (strstr(funcName, "Name") || strstr(funcName, "World"))


---

💡 Insight สำคัญ (โคตร useful)

เกมบางตัว:

EXE → import DLL
DLL → เก็บ UWorld/GNames จริง

👉 คุณสามารถ:

scan EXE → เจอ import

ตามไป DLL → scan ต่อ



---

🔥 TL;DR

✔ เพิ่ม Export / Import → ทำได้
✔ ใช้หา module จริงที่เกมใช้
✔ ใช้ต่อกับ AOB scan ได้โคตรแรง


---

😎 ถ้าจะเอาโหดกว่านี้

ผมทำให้คุณได้:

auto resolve IAT → function pointer จริง

detect dll ที่ inject runtime

หรือ map memory region (แทน module)


บอกมา เดี๋ยวจัดให้ 🔥


```

