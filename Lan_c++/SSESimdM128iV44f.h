#pragma once
#include <immintrin.h>
#include <stdint.h>
#include <type_traits>

//////////////////////////////////////////////////////////////
// Universal SIMD debug view
//////////////////////////////////////////////////////////////

union alignas(16) DBG128
{
    __m128     f;
    __m128i    i;
    __m128d    d;

    float      f32[4];
    int32_t    i32[4];
    uint64_t   u64[2];
    uintptr_t  ptr[2];
    uint8_t    bytes[16];
};

union alignas(32) DBG256
{
    __m256     f;
    __m256i    i;

    float      f32[8];
    int32_t    i32[8];
    uint64_t   u64[4];
    uintptr_t  ptr[4];
    uint8_t    bytes[32];
};

//////////////////////////////////////////////////////////////
// Generic SIMD read wrapper
//////////////////////////////////////////////////////////////

template<typename T>
struct SimdReadResult
{
    T value{};
    bool valid{};
    uintptr_t source{};
};

//////////////////////////////////////////////////////////////
// Driver bridge
//////////////////////////////////////////////////////////////

namespace SimdExt
{
    template<typename T>
    __forceinline SimdReadResult<T> Read(uintptr_t addr)
    {
        SimdReadResult<T> r{};
        r.source=addr;

        if(!addr)
            return r;

        r.value=driver.read<T>(addr);
        r.valid=true;
        return r;
    }

    __forceinline DBG128 Read128(uintptr_t addr)
    {
        DBG128 out{};
        out.i=driver.read<__m128i>(addr);
        return out;
    }

    __forceinline DBG256 Read256(uintptr_t addr)
    {
        DBG256 out{};
        out.i=driver.read<__m256i>(addr);
        return out;
    }
}

//////////////////////////////////////////////////////////////
// SIMD transform
//////////////////////////////////////////////////////////////

class alignas(64) FTransformSIMD
{
public:

    DBG128 rot;     
    DBG128 trans;   
    DBG128 scale;   

public:

    static FTransformSIMD Read(uintptr_t addr)
    {
        FTransformSIMD t{};

        t.rot   = SimdExt::Read128(addr+0x00);
        t.trans = SimdExt::Read128(addr+0x10);
        t.scale = SimdExt::Read128(addr+0x20);

        return t;
    }

    __forceinline bool IsValid() const
    {
        return
            scale.f32[0] != 0.f &&
            scale.f32[1] != 0.f &&
            scale.f32[2] != 0.f;
    }

    __forceinline __m128 Translation()
    {
        return trans.f;
    }

    __forceinline __m128 Scale()
    {
        return scale.f;
    }

    __forceinline __m128 Quaternion()
    {
        return rot.f;
    }
};

//////////////////////////////////////////////////////////////
// SSE4.1 inspection ops
//////////////////////////////////////////////////////////////

namespace SimdInspect
{
    __forceinline bool IsZero(__m128i x)
    {
        return _mm_test_all_zeros(x,x);
    }

    __forceinline bool Equal(__m128i a,__m128i b)
    {
        return _mm_test_all_ones(
            _mm_cmpeq_epi32(a,b)
        );
    }

    __forceinline __m128 Abs(__m128 x)
    {
        return _mm_andnot_ps(
            _mm_set1_ps(-0.f),
            x
        );
    }
}

//////////////////////////////////////////////////////////////
// AVX2 pointer batch scan
//////////////////////////////////////////////////////////////

struct alignas(32) PtrBatch4
{
    DBG256 batch;

    static PtrBatch4 Read(uintptr_t addr)
    {
        PtrBatch4 p{};
        p.batch=SimdExt::Read256(addr);
        return p;
    }

    uintptr_t Get(int index)
    {
        return batch.ptr[index];
    }
};

//////////////////////////////////////////////////////////////
// Runtime type reinterpret
//////////////////////////////////////////////////////////////

template<typename T>
__forceinline T SimdCast(DBG128& v)
{
    static_assert(sizeof(T)<=16);
    return *reinterpret_cast<T*>(&v);
}

/*


จุดเด่นโครงนี้
คุณได้:
อ่าน __m128i
อ่าน __m256i
เปลี่ยนชนิดดูเป็น
float
int
ptr
raw bytes
เหมาะกับพวก:
Bone transform
CameraCache
UWorld encrypted chunks
pointer arrays แบบ chunked
ตรวจโครงสร้างก่อน dereference
ถ้าจะต่อให้ใช้กับ AVX2 เช็ค pointer 4 ตัวพร้อมกัน
เช่นเทียบ valid pointers ทีเดียว

__m256i ptrs =
    driver.read<__m256i>(chunk);

__m256i zero =
    _mm256_setzero_si256();

__m256i cmp =
    _mm256_cmpeq_epi64(ptrs,zero);

if(!_mm256_testz_si256(cmp,cmp))
{
   // มี pointer ใช้งานได้
}




//เร็วมากสำหรับ array scan
//ถ้าจะทำ “DBGAllType1/2/3” แบบที่คุณคอมเมนต์ไว้
//ผมแนะนำรวมเป็น dynamic inspector:

struct SimdInspector
{
   DBG128 reg;

   float f(int i){ return reg.f32[i];}
   int i32(int i){ return reg.i32[i];}
   uintptr_t ptr(int i){ return reg.ptr[i];}
   uint8_t byte(int i){ return reg.bytes[i];}
};


//ตัวเดียวดูทุกมุม


///*///*\



