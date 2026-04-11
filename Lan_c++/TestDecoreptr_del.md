```cpp


uintptr_t NormalizeHighBits(uintptr_t addr)
{
	// ----------------------------------------
	// STEP 1: ถ้ามี high bits (เกิน 48-bit)
	// ----------------------------------------
	if (addr >> 48)
	{
		// ดึง byte บนสุด (key candidate)
		uint8_t key1 = (addr >> 56) & 0xFF;

		// ดึง byte รอง (บางเกมใช้เป็น key2)
		uint8_t key2 = (addr >> 48) & 0xFF;

		uintptr_t cleaned = addr;

		// ----------------------------------------
		// CASE 1: XOR ด้วย key1 ทั้ง pointer
		// ----------------------------------------
		if (key1 != 0)
		{
			uintptr_t mask = 0;
			for (int i = 0; i < 8; i++)
				mask |= (uintptr_t)key1 << (i * 8);

			uintptr_t test = addr ^ mask;

			// ถ้า decode แล้วกลายเป็น canonical pointer → ใช้
			if ((test >> 48) == 0)
				return test;
		}

		// ----------------------------------------
		// CASE 2: XOR key1 + key2 (2-layer)
		// ----------------------------------------
		if (key1 != 0 || key2 != 0)
		{
			uintptr_t mask1 = 0;
			uintptr_t mask2 = 0;

			for (int i = 0; i < 8; i++)
			{
				mask1 |= (uintptr_t)key1 << (i * 8);
				mask2 |= (uintptr_t)key2 << (i * 8);
			}

			uintptr_t test = addr ^ mask1 ^ mask2;

			if ((test >> 48) == 0)
				return test;
		}

		// ----------------------------------------
		// CASE 3: fallback → ตัด high bits ทิ้ง
		// ----------------------------------------
		cleaned &= 0x0000FFFFFFFFFFFFULL;
		return cleaned;
	}

	return addr;
}
uintptr_t TrimTrailingNibbleEp2(uintptr_t addr)
{
	uint8_t lowByte = static_cast<uint8_t>(addr & 0xFFu);

	uint8_t highNibble = (lowByte & 0xF0u) >> 4;
	uint8_t lowNibble = (lowByte & 0x0Fu);

	// ถ้า pattern 0x0X → ลบทิ้ง nibble ล่างแทน
	if (highNibble == 0x0 && lowNibble >= 1 && lowNibble <= 0xF)
	{
		// ❗ แทน shift → clear แค่ 4 bit ล่าง
		return (addr & ~0xFULL);
	}

	return addr;
}
uintptr_t TrimTrailingNibble(uintptr_t addr)
{
	// 🔥 ตัด high garbage ก่อน
	addr = NormalizeHighBits(addr);

	// 🔥 fix nibble ล่าง
	addr = TrimTrailingNibbleEp2(addr);

	//-------------------------------------------------------------
	// MSB scan logic (ของเดิม)
	//-------------------------------------------------------------
	unsigned long msb = 0;
	if (_BitScanReverse64(&msb, addr))
	{
		int msNib = msb >> 2;

		if (msNib >= 3)
		{
			WORD group16 = (WORD)((addr >> ((msNib - 3) * 4)) & 0xFFFF);

			BYTE nib0 = (group16 >> 12) & 0xF;
			BYTE nib1 = (group16 >> 8) & 0xF;
			BYTE nib2 = (group16 >> 4) & 0xF;
			BYTE nib3 = (group16) & 0xF;

			// pattern X000
			if (nib0 != 0 && nib1 == 0 && nib2 == 0 && nib3 == 0)
			{
				uintptr_t mask = ~(uintptr_t(0xFULL) << (msNib * 4));
				addr &= mask;
			}
		}
	}

	return addr;
}


```
