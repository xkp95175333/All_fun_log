
// don't test 
using MemAll10Type = std::variant<
	float,
	double,
	int8_t,
	int16_t,
	int32_t,
	intptr_t,
	uint8_t,
	uint16_t,
	uint32_t,
	uintptr_t
>;

//inclass
template<typename T>
T read(uintptr_t address)
{
	T buffer{};
	readmemory((PVOID)address, &buffer, sizeof(T));
	return buffer;
}
template<typename T>
T read128Ex2(uintptr_t address) 
{
	T buffer{};
	readmemoryEx2((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

std::vector<uint8_t> ReadBlock(uint64_t address, size_t size)
{
	std::vector<uint8_t> out(size);
	if (address == 0 || size == 0)
		return out;

	driver.readmnew(address, out.data(), (uint32_t)size);
	return out;
}

std::vector<unsigned char> ReadBlock_unsigC(uint64_t address, size_t size)
{
	std::vector<unsigned char> out(size);
	if (address == 0 || size == 0)
		return out;

	driver.readmnew(address, out.data(), (uint32_t)size);
	return out;
}


template<typename T = __m128i>
std::string readstring128Ex2(uintptr_t address, size_t size)
{
	std::string result;
	result.reserve(size);

	// อ่านเป็น chunk ละ 16 bytes
	for (size_t offset = 0; offset < size; offset += sizeof(__m128i))
	{
		__m128i chunk{};
		if (!driver.readmemoryEx2((PVOID)(address + offset), &chunk, sizeof(__m128i)))
			break; // ถ้าอ่านล้มเหลว หยุดเลย

		// แปลง __m128i -> bytes
		alignas(16) unsigned char bytes[16];
		_mm_store_si128((__m128i*)bytes, chunk);

		// เพิ่มเข้า string ตามขนาดจริง (กัน '\0' ตอนท้าย)
		size_t remaining = size - offset;
		size_t copySize = remaining < sizeof(bytes) ? remaining : sizeof(bytes);
		result.append((char*)bytes, copySize);
	}

	// ตัด '\0' ที่อาจจะอยู่ตอนท้ายออก
	if (!result.empty() && result.back() == '\0')
		result.pop_back();

	return result;
}
