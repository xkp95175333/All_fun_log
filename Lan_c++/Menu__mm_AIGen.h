#pragma once

#include <Windows.h>
#include <immintrin.h>

#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

//==============================================================
// SIMD MENU
//==============================================================

enum class menuGet : uint32_t
{
    //----------------------------------------------------------
    // STORE
    //----------------------------------------------------------

    StoreAligned,
    StoreUnaligned,
    StoreLow64,

    //----------------------------------------------------------
    // XOR
    //----------------------------------------------------------

    XOR128,
    XOR64,
    XOR32,

    //----------------------------------------------------------
    // SHIFT
    //----------------------------------------------------------

    ShiftLeftBytes,
    ShiftRightBytes,

    ShiftLeft32,
    ShiftRight32,

    ShiftLeft64,
    ShiftRight64,

    //----------------------------------------------------------
    // ADD
    //----------------------------------------------------------

    Add8,
    Add16,
    Add32,
    Add64,

    //----------------------------------------------------------
    // SUB
    //----------------------------------------------------------

    Sub8,
    Sub16,
    Sub32,
    Sub64,

    //----------------------------------------------------------
    // AND OR
    //----------------------------------------------------------

    And128,
    Or128,

    //----------------------------------------------------------
    // COMPARE
    //----------------------------------------------------------

    Compare8,
    Compare16,
    Compare32,

    //----------------------------------------------------------
    // SHUFFLE
    //----------------------------------------------------------

    Shuffle32,
    Shuffle8,

    //----------------------------------------------------------
    // ROTATE STYLE
    //----------------------------------------------------------

    RotateLeft32,
    RotateRight32,

    RotateLeft64,
    RotateRight64,

    //----------------------------------------------------------
    // UNPACK
    //----------------------------------------------------------

    UnpackLow8,
    UnpackHigh8,

    //----------------------------------------------------------
    // MISC
    //----------------------------------------------------------

    Zero,
    Set1_8,
    Set1_16,
    Set1_32,
    Set1_64,
};

//==============================================================
// SIMD CLASS
//==============================================================

class SIMD128
{
public:

    __m128i value;

public:

    //----------------------------------------------------------
    // CTOR
    //----------------------------------------------------------

    SIMD128()
    {
        value = _mm_setzero_si128();
    }

    SIMD128(__m128i v)
    {
        value = v;
    }

    SIMD128(uint64_t low, uint64_t high = 0)
    {
        value = _mm_set_epi64x(high, low);
    }

    //----------------------------------------------------------
    // LOAD
    //----------------------------------------------------------

    inline void LoadAligned(const void* ptr)
    {
        value = _mm_load_si128((__m128i*)ptr);
    }

    inline void LoadUnaligned(const void* ptr)
    {
        value = _mm_loadu_si128((__m128i*)ptr);
    }

    //----------------------------------------------------------
    // STORE
    //----------------------------------------------------------

    inline void StoreAligned(void* out)
    {
        _mm_store_si128((__m128i*)out, value);
    }

    inline void StoreUnaligned(void* out)
    {
        _mm_storeu_si128((__m128i*)out, value);
    }

    inline void StoreLow64(void* out)
    {
        _mm_storel_epi64((__m128i*)out, value);
    }

    //----------------------------------------------------------
    // XOR
    //----------------------------------------------------------

    inline void XOR(__m128i other)
    {
        value = _mm_xor_si128(value, other);
    }

    inline void XOR64(uint64_t v)
    {
        value = _mm_xor_si128(
            value,
            _mm_set1_epi64x(v));
    }

    inline void XOR32(uint32_t v)
    {
        value = _mm_xor_si128(
            value,
            _mm_set1_epi32(v));
    }

    //----------------------------------------------------------
    // AND OR
    //----------------------------------------------------------

    inline void AND(__m128i other)
    {
        value = _mm_and_si128(value, other);
    }

    inline void OR(__m128i other)
    {
        value = _mm_or_si128(value, other);
    }

    //----------------------------------------------------------
    // ADD
    //----------------------------------------------------------

    inline void Add8(__m128i other)
    {
        value = _mm_add_epi8(value, other);
    }

    inline void Add16(__m128i other)
    {
        value = _mm_add_epi16(value, other);
    }

    inline void Add32(__m128i other)
    {
        value = _mm_add_epi32(value, other);
    }

    inline void Add64(__m128i other)
    {
        value = _mm_add_epi64(value, other);
    }

    //----------------------------------------------------------
    // SUB
    //----------------------------------------------------------

    inline void Sub8(__m128i other)
    {
        value = _mm_sub_epi8(value, other);
    }

    inline void Sub16(__m128i other)
    {
        value = _mm_sub_epi16(value, other);
    }

    inline void Sub32(__m128i other)
    {
        value = _mm_sub_epi32(value, other);
    }

    inline void Sub64(__m128i other)
    {
        value = _mm_sub_epi64(value, other);
    }

    //----------------------------------------------------------
    // SHIFT
    //----------------------------------------------------------

    inline void ShiftLeftBytes(int count)
    {
        value = _mm_slli_si128(value, count);
    }

    inline void ShiftRightBytes(int count)
    {
        value = _mm_srli_si128(value, count);
    }

    inline void ShiftLeft32(int count)
    {
        value = _mm_slli_epi32(value, count);
    }

    inline void ShiftRight32(int count)
    {
        value = _mm_srli_epi32(value, count);
    }

    inline void ShiftLeft64(int count)
    {
        value = _mm_slli_epi64(value, count);
    }

    inline void ShiftRight64(int count)
    {
        value = _mm_srli_epi64(value, count);
    }

    //----------------------------------------------------------
    // ROTATE
    //----------------------------------------------------------

    inline void RotateLeft32(int bits)
    {
        __m128i a = _mm_slli_epi32(value, bits);
        __m128i b = _mm_srli_epi32(value, 32 - bits);

        value = _mm_or_si128(a, b);
    }

    inline void RotateRight32(int bits)
    {
        __m128i a = _mm_srli_epi32(value, bits);
        __m128i b = _mm_slli_epi32(value, 32 - bits);

        value = _mm_or_si128(a, b);
    }

    inline void RotateLeft64(int bits)
    {
        __m128i a = _mm_slli_epi64(value, bits);
        __m128i b = _mm_srli_epi64(value, 64 - bits);

        value = _mm_or_si128(a, b);
    }

    inline void RotateRight64(int bits)
    {
        __m128i a = _mm_srli_epi64(value, bits);
        __m128i b = _mm_slli_epi64(value, 64 - bits);

        value = _mm_or_si128(a, b);
    }

    //----------------------------------------------------------
    // SHUFFLE
    //----------------------------------------------------------

    inline void Shuffle32(int imm)
    {
        value = _mm_shuffle_epi32(value, imm);
    }

    inline void Shuffle8(__m128i mask)
    {
        value = _mm_shuffle_epi8(value, mask);
    }

    //----------------------------------------------------------
    // COMPARE
    //----------------------------------------------------------

    inline __m128i Compare8(__m128i other)
    {
        return _mm_cmpeq_epi8(value, other);
    }

    inline __m128i Compare16(__m128i other)
    {
        return _mm_cmpeq_epi16(value, other);
    }

    inline __m128i Compare32(__m128i other)
    {
        return _mm_cmpeq_epi32(value, other);
    }

    //----------------------------------------------------------
    // UNPACK
    //----------------------------------------------------------

    inline void UnpackLow8(__m128i other)
    {
        value = _mm_unpacklo_epi8(value, other);
    }

    inline void UnpackHigh8(__m128i other)
    {
        value = _mm_unpackhi_epi8(value, other);
    }

    //----------------------------------------------------------
    // SET
    //----------------------------------------------------------

    inline void Zero()
    {
        value = _mm_setzero_si128();
    }

    inline void Set1_8(int8_t v)
    {
        value = _mm_set1_epi8(v);
    }

    inline void Set1_16(int16_t v)
    {
        value = _mm_set1_epi16(v);
    }

    inline void Set1_32(int32_t v)
    {
        value = _mm_set1_epi32(v);
    }

    inline void Set1_64(int64_t v)
    {
        value = _mm_set1_epi64x(v);
    }

    //----------------------------------------------------------
    // RAW
    //----------------------------------------------------------

    inline __m128i Get() const
    {
        return value;
    }

    //----------------------------------------------------------
    // HEX STRING
    //----------------------------------------------------------

    inline std::string ToHexString()
    {
        alignas(16) uint8_t bytes[16];

        _mm_store_si128((__m128i*)bytes, value);

        std::stringstream ss;

        for (int i = 15; i >= 0; --i)
        {
            ss << std::hex
               << std::setw(2)
               << std::setfill('0')
               << (int)bytes[i]
               << " ";
        }

        return ss.str();
    }
};

//ตัวอย่างใช้:

alignas(16) uint8_t data[16] =
{
    1,2,3,4,
    5,6,7,8,
    9,10,11,12,
    13,14,15,16
};

SIMD128 simd;

simd.LoadAligned(data);

simd.XOR32(0xDEADBEEF);

simd.RotateLeft32(5);

simd.Shuffle32(_MM_SHUFFLE(2,1,0,3));

printf("%s\n", simd.ToHexString().c_str());
//หรือใช้ encrypt runtime:
SIMD128 crypt(
    0x1122334455667788,
    0x99AABBCCDDEEFF00);

crypt.XOR64(0xCAFEBABE);

crypt.RotateRight64(13);

crypt.ShiftLeftBytes(2);




//Type m512i 

#pragma once

#include <Windows.h>
#include <immintrin.h>

#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>

//==============================================================
// AVX512 CLASS
//==============================================================

class SIMD512
{
public:

    __m512i value;

public:

    //----------------------------------------------------------
    // CTOR
    //----------------------------------------------------------

    SIMD512()
    {
        value = _mm512_setzero_si512();
    }

    SIMD512(__m512i v)
    {
        value = v;
    }

    //----------------------------------------------------------
    // LOAD
    //----------------------------------------------------------

    inline void LoadAligned(const void* ptr)
    {
        value =
            _mm512_load_si512(ptr);
    }

    inline void LoadUnaligned(const void* ptr)
    {
        value =
            _mm512_loadu_si512(ptr);
    }

    //----------------------------------------------------------
    // STORE
    //----------------------------------------------------------

    inline void StoreAligned(void* out)
    {
        _mm512_store_si512(out, value);
    }

    inline void StoreUnaligned(void* out)
    {
        _mm512_storeu_si512(out, value);
    }

    //----------------------------------------------------------
    // ZERO
    //----------------------------------------------------------

    inline void Zero()
    {
        value =
            _mm512_setzero_si512();
    }

    //----------------------------------------------------------
    // SET
    //----------------------------------------------------------

    inline void Set1_8(int8_t v)
    {
        value =
            _mm512_set1_epi8(v);
    }

    inline void Set1_16(int16_t v)
    {
        value =
            _mm512_set1_epi16(v);
    }

    inline void Set1_32(int32_t v)
    {
        value =
            _mm512_set1_epi32(v);
    }

    inline void Set1_64(int64_t v)
    {
        value =
            _mm512_set1_epi64(v);
    }

    //----------------------------------------------------------
    // XOR
    //----------------------------------------------------------

    inline void XOR(__m512i other)
    {
        value =
            _mm512_xor_si512(
                value,
                other);
    }

    inline void XOR64(uint64_t v)
    {
        value =
            _mm512_xor_si512(
                value,
                _mm512_set1_epi64(v));
    }

    inline void XOR32(uint32_t v)
    {
        value =
            _mm512_xor_si512(
                value,
                _mm512_set1_epi32(v));
    }

    //----------------------------------------------------------
    // AND OR
    //----------------------------------------------------------

    inline void AND(__m512i other)
    {
        value =
            _mm512_and_si512(
                value,
                other);
    }

    inline void OR(__m512i other)
    {
        value =
            _mm512_or_si512(
                value,
                other);
    }

    //----------------------------------------------------------
    // ADD
    //----------------------------------------------------------

    inline void Add8(__m512i other)
    {
        value =
            _mm512_add_epi8(
                value,
                other);
    }

    inline void Add16(__m512i other)
    {
        value =
            _mm512_add_epi16(
                value,
                other);
    }

    inline void Add32(__m512i other)
    {
        value =
            _mm512_add_epi32(
                value,
                other);
    }

    inline void Add64(__m512i other)
    {
        value =
            _mm512_add_epi64(
                value,
                other);
    }

    //----------------------------------------------------------
    // SUB
    //----------------------------------------------------------

    inline void Sub8(__m512i other)
    {
        value =
            _mm512_sub_epi8(
                value,
                other);
    }

    inline void Sub16(__m512i other)
    {
        value =
            _mm512_sub_epi16(
                value,
                other);
    }

    inline void Sub32(__m512i other)
    {
        value =
            _mm512_sub_epi32(
                value,
                other);
    }

    inline void Sub64(__m512i other)
    {
        value =
            _mm512_sub_epi64(
                value,
                other);
    }

    //----------------------------------------------------------
    // SHIFT
    //----------------------------------------------------------

    inline void ShiftLeft32(int bits)
    {
        value =
            _mm512_slli_epi32(
                value,
                bits);
    }

    inline void ShiftRight32(int bits)
    {
        value =
            _mm512_srli_epi32(
                value,
                bits);
    }

    inline void ShiftLeft64(int bits)
    {
        value =
            _mm512_slli_epi64(
                value,
                bits);
    }

    inline void ShiftRight64(int bits)
    {
        value =
            _mm512_srli_epi64(
                value,
                bits);
    }

    //----------------------------------------------------------
    // ROTATE
    //----------------------------------------------------------

    inline void RotateLeft32(int bits)
    {
        __m512i a =
            _mm512_slli_epi32(value, bits);

        __m512i b =
            _mm512_srli_epi32(value, 32 - bits);

        value =
            _mm512_or_si512(a, b);
    }

    inline void RotateRight32(int bits)
    {
        __m512i a =
            _mm512_srli_epi32(value, bits);

        __m512i b =
            _mm512_slli_epi32(value, 32 - bits);

        value =
            _mm512_or_si512(a, b);
    }

    inline void RotateLeft64(int bits)
    {
        __m512i a =
            _mm512_slli_epi64(value, bits);

        __m512i b =
            _mm512_srli_epi64(value, 64 - bits);

        value =
            _mm512_or_si512(a, b);
    }

    inline void RotateRight64(int bits)
    {
        __m512i a =
            _mm512_srli_epi64(value, bits);

        __m512i b =
            _mm512_slli_epi64(value, 64 - bits);

        value =
            _mm512_or_si512(a, b);
    }

    //----------------------------------------------------------
    // COMPARE
    //----------------------------------------------------------

    inline __mmask64 Compare8(__m512i other)
    {
        return
            _mm512_cmpeq_epi8_mask(
                value,
                other);
    }

    inline __mmask32 Compare16(__m512i other)
    {
        return
            _mm512_cmpeq_epi16_mask(
                value,
                other);
    }

    inline __mmask16 Compare32(__m512i other)
    {
        return
            _mm512_cmpeq_epi32_mask(
                value,
                other);
    }

    inline __mmask8 Compare64(__m512i other)
    {
        return
            _mm512_cmpeq_epi64_mask(
                value,
                other);
    }

    //----------------------------------------------------------
    // SHUFFLE
    //----------------------------------------------------------

    inline void Shuffle32(__m512i idx)
    {
        value =
            _mm512_permutexvar_epi32(
                idx,
                value);
    }

    inline void Shuffle64(__m512i idx)
    {
        value =
            _mm512_permutexvar_epi64(
                idx,
                value);
    }

    //----------------------------------------------------------
    // BLEND
    //----------------------------------------------------------

    inline void Blend32(
        __m512i other,
        __mmask16 mask)
    {
        value =
            _mm512_mask_blend_epi32(
                mask,
                value,
                other);
    }

    //----------------------------------------------------------
    // MIN MAX
    //----------------------------------------------------------

    inline void Min32(__m512i other)
    {
        value =
            _mm512_min_epi32(
                value,
                other);
    }

    inline void Max32(__m512i other)
    {
        value =
            _mm512_max_epi32(
                value,
                other);
    }

    //----------------------------------------------------------
    // RAW
    //----------------------------------------------------------

    inline __m512i Get() const
    {
        return value;
    }

    //----------------------------------------------------------
    // HEX STRING
    //----------------------------------------------------------

    inline std::string ToHexString()
    {
        alignas(64) uint8_t bytes[64];

        _mm512_store_si512(
            bytes,
            value);

        std::stringstream ss;

        for (int i = 63; i >= 0; --i)
        {
            ss << std::hex
               << std::setw(2)
               << std::setfill('0')
               << (int)bytes[i]
               << " ";

            if ((i % 16) == 0)
                ss << "\n";
        }

        return ss.str();
    }
};
//ตัวอย่างใช้:
alignas(64) uint8_t buffer[64]{};

SIMD512 simd;

simd.LoadAligned(buffer);

simd.XOR64(0xDEADBEEFCAFEBABE);

simd.RotateLeft64(13);

simd.ShiftRight32(3);

printf("%s\n",
    simd.ToHexString().c_str());

//เช็ค CPU ก่อนใช้ AVX512:
bool HasAVX512()
{
    int cpuInfo[4]{};

    __cpuidex(cpuInfo, 7, 0);

    return (cpuInfo[1] & (1 << 16)) != 0;
}
//compile flags:
/arch:AVX512
//หรือ GCC/Clang:
-mavx512f
  

