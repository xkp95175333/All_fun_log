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

---
Loacltype __m  SSE SSE2 SSE 4.1 AVX 2 
---
```cpp
//128
typedef union __declspec(intrin_type) __declspec(align(16)) __m128i {
    __int8              m128i_i8[16];
    __int16             m128i_i16[8];
    __int32             m128i_i32[4];
    __int64             m128i_i64[2];
    unsigned __int8     m128i_u8[16];
    unsigned __int16    m128i_u16[8];
    unsigned __int32    m128i_u32[4];
    unsigned __int64    m128i_u64[2];
} __m128i;

//512
typedef union  __declspec(intrin_type) __declspec(align(64)) __m512i {
    __int8              m512i_i8[64];
    __int16             m512i_i16[32];
    __int32             m512i_i32[16];
    __int64             m512i_i64[8];
    unsigned __int8     m512i_u8[64];
    unsigned __int16    m512i_u16[32];
    unsigned __int32    m512i_u32[16];
    unsigned __int64    m512i_u64[8];
} __m512i;

```


---
Decore Don't test
---
```cpp

struct SimdConsts
{
	__m128 A0;
	__m128 B0;
	__m128 DE0;
	__m128 C090;
};
SimdConsts SetupKeyLavel(uintptr_t BaseWork)
{
	SimdConsts Out = {};
	Out.A0 = driver.read< __m128>(BaseWork + AAOFF_A0);
	Out.B0 = driver.read< __m128>(BaseWork + AAOFF_B0);
	Out.DE0 = driver.read< __m128>(BaseWork + AAOFF_DE0);
	Out.C090 = driver.read< __m128>(BaseWork + AAOFF_090);
	return Out;
}
inline SimdConsts LoadConsts(uintptr_t base)
{
	SimdConsts c;

	c.A0 = _mm_loadu_ps((float*)GET_PTR(base, OFF_A0));
	c.B0 = _mm_loadu_ps((float*)GET_PTR(base, OFF_B0));
	c.DE0 = _mm_loadu_ps((float*)GET_PTR(base, OFF_DE0));
	c.C090 = _mm_loadu_ps((float*)GET_PTR(base, OFF_090));

	return c;
}
inline __m128 NormalizeRotationEx(__m128 v, const SimdConsts& c)
{
	__m128 div = _mm_div_ps(v, c.A0);

	__m128 i = _mm_cvtepi32_ps(_mm_cvttps_epi32(div));

	__m128 sub = _mm_sub_ps(
		v,
		_mm_mul_ps(
			_mm_xor_ps(
				_mm_and_ps(
					_mm_cmple_ps(c.B0,
						_mm_and_ps(div, c.DE0)),
					_mm_xor_ps(i, div)
				),
				i
			),
			c.A0
		)
	);

	__m128 add = _mm_add_ps(c.A0, sub);

	__m128 fix1 = _mm_xor_ps(
		_mm_and_ps(_mm_xor_ps(add, sub),
			_mm_cmple_ps(_mm_setzero_ps(), sub)),
		add
	);

	__m128 fix2 = _mm_xor_ps(
		_mm_and_ps(
			_mm_xor_ps(_mm_sub_ps(fix1, c.A0), fix1),
			_mm_cmplt_ps(c.C090, fix1)
		),
		fix1
	);

	return fix2;
}

uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets)
{
	uint64_t result = driver.read<uint64_t>(base + offsets.at(0));
	for (int i = 1; i < offsets.size(); i++) {
		result = driver.read<uint64_t>(result + offsets.at(i));
	}
	return result;
}
uintptr_t DecodePointerSmart(uint64_t a, uint64_t b, uint64_t c = 0, uint64_t d = 0)
{
	uintptr_t candidates[] =
	{
		a, b, c, d,

		a ^ b,
		b ^ c,
		c ^ d,

		a + b,
		b + c,
		c + d,

		a - b,
		b - c,

		(a ^ b ^ c),
		(b ^ c ^ d)
	};

	for (auto& v : candidates)
	{
		v = NormalizePtrSmart(v);

		if (v > 0x10000 && v < 0x7FFFFFFFFFFF)
			return v;
	}

	return 0;
}

struct RootAllType
{
	uint64_t p1, p2, p3, p4 ;
};
uintptr_t GetbasicRootComponent(uintptr_t actor, uint32_t OffsetStart = 0x1C0)
{
	RootAllType Menbasic = driver.read<RootAllType>(actor + OffsetStart);
	uintptr_t rc = DecodePointerSmart(Menbasic.p1, Menbasic.p2, Menbasic.p3, Menbasic.p4);
	return rc;
}

uintptr_t GetRootComponent(uintptr_t actor ,uint32_t OffsetStart= 0x1C0)
{ 
	__m512i Men = driver.read<__m512i>(actor + OffsetStart);

	 
	uint64_t p1 = driver.read<uint64_t>(actor + OffsetStart);
	uint64_t p2 = driver.read<uint64_t>(actor + (OffsetStart+8));
	uint64_t p3 = driver.read<uint64_t>(actor + (OffsetStart + 16));
	uint64_t p4 = driver.read<uint64_t>(actor + (OffsetStart + 24));

	uintptr_t rc = DecodePointerSmart(p1, p2, p3, p4);

	return rc;
}


std::vector<unsigned char> ReadBlockDecodedSmart(uint64_t addr, size_t size)
{
	std::vector<unsigned char> out(size);
	if (!addr || size == 0) return out;

	// อ่าน raw
	driver.readmnew(addr, out.data(), (uint32_t)size);

	// =========================
	// 🔥 PASS: decode pointer chain
	// =========================
	for (size_t i = 0; i + 0x20 <= size; i += 8)
	{
		uint64_t a = *(uint64_t*)(&out[i + 0x00]);
		uint64_t b = *(uint64_t*)(&out[i + 0x08]);
		uint64_t c = *(uint64_t*)(&out[i + 0x10]);
		uint64_t d = *(uint64_t*)(&out[i + 0x18]);

		uintptr_t decoded = DecodePointerSmart(a, b, c, d);

		if (decoded)
		{
			*(uintptr_t*)(&out[i]) = decoded;
		}
	}

	// =========================
	// 🔥 PASS: fallback (ของเดิมมึง)
	// =========================
	for (size_t i = 0; i + 8 <= size; i += 8)
	{
		uintptr_t v = *(uintptr_t*)(&out[i]);

		if (v > 0x1000)
		{
			v = NormalizePtrSmart(v);
			*(uintptr_t*)(&out[i]) = v;
		}
	}

	return out;
}

uintptr_t DecodeBest(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
{
	uintptr_t best = 0;
	int scoreBest = 0;

	uintptr_t tries[] =
	{
		a, b, c, d,
		a ^ b, b ^ c, c ^ d,
		a + b, b + c,
		a ^ b ^ c,
		b ^ c ^ d
	};

	for (auto t : tries)
	{
		t = NormalizePtrSmart(t);

		int score = 0;

		if (t > 0x10000) score++;
		if ((t & 0xFFF) != 0) score++;
		if ((t >> 40) != 0) score++;

		if (score > scoreBest)
		{
			scoreBest = score;
			best = t;
		}
	}

	return best;
}

```
