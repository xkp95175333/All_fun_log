
std::vector<uint8_t> ReadBlock(uint64_t address, size_t size)
{
	std::vector<uint8_t> out(size);
	if (address == 0 || size == 0)
		return out;

	driver.readmnew(address, out.data(), (uint32_t)size);
	return out;
}

uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets)
{
	uint64_t result = driver.read<uint64_t>(base + offsets.at(0));
	for (int i = 1; i < offsets.size(); i++) {
		result = driver.read<uint64_t>(result + offsets.at(i));
	}
	return result;
} 


 
namespace setupimguidebug
{

	bool AutoupdateUorld = false;
	uint64_t baseaddress = NULL;
	size_t moduleSize = 0;
	uint32_t base_Size = 0;
	uint64_t endAddress = NULL;
	std::vector<unsigned char> memoryData; //เก็บ bytes ทั้ง module[0] เฉพราะใน Process 

}

uint64_t SearchAOBFindUEnew(
    const std::string& aobPattern)
{
    std::vector<uint8_t> pattern;
    std::vector<bool> mask;

    std::istringstream iss(aobPattern);
    std::string byteStr;

    while (iss >> byteStr)
    {
        if(byteStr=="?"||
           byteStr=="??")
        {
            pattern.push_back(0);
            mask.push_back(false);
        }
        else
        {
            pattern.push_back(
                (uint8_t)strtoul(
                    byteStr.c_str(),
                    nullptr,
                    16
                )
            );

            mask.push_back(true);
        }
    }

    if(pattern.empty())
        return 0;

    if(setupimguidebug::
        memoryData.size()
        <pattern.size())
        return 0;

    const size_t last=
        setupimguidebug::
        memoryData.size()
        -
        pattern.size();

    for(size_t i=0;i<=last;i++)
    {
        bool found=true;

        for(size_t j=0;
            j<pattern.size();
            j++)
        {
            if(mask[j] &&
              setupimguidebug::
              memoryData[i+j]
              !=pattern[j])
            {
                found=false;
                break;
            }
        }

        if(found)
        {
            return
            setupimguidebug::
            baseaddress+i;
        }
    }

    return 0;
}

uint64_t SearchAOBFindUE(const std::string aobPattern) {
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
	for (size_t i = 0; i < setupimguidebug::memoryData.size() - patternSize; ++i)
	{
		bool found = true;
		for (size_t j = 0; j < patternSize; ++j) {
			if (mask[j] && setupimguidebug::memoryData[i + j] != pattern[j]) {
				found = false;
				break;
			}
		}
		if (found) {

			return setupimguidebug::baseaddress + i;

		}
	}

}
uint64_t CalculateRip(uint64_t instructionAddress)
{
    if (!instructionAddress)
        return 0;

    int32_t rel =
        driver.read<int32_t>(
            instructionAddress + 3
        );

    return instructionAddress + 7 + rel;
}

uint64_t CalculateUworldAddress(uint64_t value, uint64_t base) {
	if (value == 0) return 0;
	uint64_t addrtoUwrold = value + 3;
	uint32_t relativeOffset = driver.read<uint32_t>(addrtoUwrold);
	if (relativeOffset == 0) return 0;
	return addrtoUwrold + 4 + relativeOffset;
}


void GetdataOffset() {
	IMAGE_DOS_HEADER dosHeader = driver.read<IMAGE_DOS_HEADER>(driver.base_address);
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		printf(_("Invalid DOS Header signature!\n"));
		return;
	}

	IMAGE_NT_HEADERS ntHeaders = driver.read<IMAGE_NT_HEADERS>(driver.base_address + dosHeader.e_lfanew);
	if (ntHeaders.OptionalHeader.SizeOfImage == 0) {
		printf(_("Failed to read module size!\n"));
		return;
	}

	setupimguidebug::base_Size = ntHeaders.OptionalHeader.SizeOfImage;
	printf(_("Sig->0x%08X modSize: %08X\n"), ntHeaders.Signature, setupimguidebug::base_Size);

	// อ่านทีละ Block แทนการโหลดทั้งหมด
	setupimguidebug::moduleSize = setupimguidebug::base_Size;
	setupimguidebug::baseaddress = driver.base_address;
	setupimguidebug::endAddress = setupimguidebug::baseaddress + setupimguidebug::moduleSize - 1; // Address สุดท้ายของโมดูล


	setupimguidebug::memoryData.resize(setupimguidebug::moduleSize);

	size_t totalRead = 0;
	unsigned char buffer[4096] = { 0 };
	for (size_t i = 0; i < setupimguidebug::moduleSize; i += sizeof(buffer)) {
		size_t readSize = min(sizeof(buffer), setupimguidebug::moduleSize - i);
		driver.readmnew(setupimguidebug::baseaddress + i, buffer, readSize);
		std::copy(buffer, buffer + readSize, setupimguidebug::memoryData.begin() + i);
		totalRead += readSize;

	}
	printf(_("memoryData Size: %08X\n"), setupimguidebug::memoryData.size());


	 
	static const char AobWorld[] = "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81";
	static const char AobReplayInterface[] = "48 8D 0D ? ? ? ? 89 ? ? ? E8 ? ? ? ? 48 83 ? ? ? 48 8B 05"; 
	static const char AobW2s[] = "48 89 5C 24 ? 55 56 57 48 83 EC 70 65 4C 8B 0c 25";
	static const char AobBonePos[] = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 60 48 8B 01 41 8B E8 48 8B F2 48 8B F9 33 DB";
	static const char AobCamera[] = "48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB";
	
	auto valueworld = SearchAOBFindUE(AobWorld);
	if(!valueworld)valueworld = SearchAOBFindUE("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 ? ? 8B 81 ? ? ? ? C1 ? ? A8 ? ? ? 48 8B 81 ? ? ? ? EB ? 33 ? 48 85");
	printf(_("world 0x%llX\n"), valueworld);

	auto UworldAddress = CalculateUworldAddress(valueworld, driver.base_address);
	uint32_t OffsetWorld = UworldAddress - driver.base_address;
	printf(_("ptr world Address: 0x%llX || 0x%llX\n"), UworldAddress, OffsetWorld);
 

	auto valueCamera = SearchAOBFindUE(AobCamera); 
	printf(_("Camera 0x%llX\n"), valueCamera);
	auto CameraAddress = CalculateUworldAddress(valueCamera, driver.base_address);
	uint32_t OffsetCamera = CameraAddress - driver.base_address;
	printf(_("ptr Camera Address: 0x%llX || 0x%llX\n"), CameraAddress, OffsetCamera);




	setupimguidebug::memoryData.clear();
};
