---
include  Kernel 
---
```c
//cr3

#define IMAGE_SCN_MEM_EXECUTE 0x20000000

#define MAXLONGLONG                         (0x7fffffffffffffff)

#define DECLSPEC_CACHEALIGN DECLSPEC_ALIGN(SYSTEM_CACHE_ALIGNMENT_SIZE)

#define IMAGE_FIRST_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)((ULONG_PTR)(ntheader) + FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader) + ((ntheader))->FileHeader.SizeOfOptionalHeader))
#define to_lower(text) ((text >= (char*)'A' && text <= (char*)'Z') ? (text + 32) : text)
#define v32(address) ((((unsigned __int64)address^ ((unsigned __int64)address << 13)) >> 7) ^ (unsigned __int64)address^ ((unsigned __int64)address << 13))
#define v33(address) (v32(address) ^ (v32(address) << 17))
#define decrypt_cr3(cr3, key, address) (cr3 & 0xBFFF000000000FFF | (((key ^ v33(address) ^ (v33(address) << 32)) & 0xFFFFFFFFF) << 12)) 
#define CHECK_PTR(ptr) if (!(ptr)) { logChack("Pointer " #ptr " is NULL\n"); return STATUS_UNSUCCESSFUL; }
#define win10_1803 17134
#define win10_1809 17763
#define win10_1903 18362
#define win10_1909 18363
#define win10_2004 19041
#define win10_20h2 19042
#define win10_21h1 19043
#define win10_21h2 19044
#define win10_22h2 19045

#define win11_21h2 22000
#define win11_22h2 22621
#define win11_23h2 22631

#define win11_24h2 26100
#define win11_24h2_Fbuild 26241 
#define page_offset_size 12





// Mode SSE SSE2 SSE41 
#include <intrin.h>

#include <emmintrin.h>   // SSE2
#include <xmmintrin.h>   // SSE
#include <smmintrin.h>   // SSE4.1
```
---
Core ตรวจสอบ ว่า ใช้งาน ได้ไหม 
---
```c


__forceinline bool CpuHasSSE()
{
	int cpuInfo[4]{};
	__cpuid(cpuInfo, 1);
	return (cpuInfo[3] & (1 << 25)) != 0;
}

__forceinline bool CpuHasSSE2()
{
	int cpuInfo[4]{};
	__cpuid(cpuInfo, 1);
	return (cpuInfo[3] & (1 << 26)) != 0;
}

__forceinline bool CpuHasSSE41()
{
	int cpuInfo[4]{};
	__cpuid(cpuInfo, 1);
	return (cpuInfo[2] & (1 << 19)) != 0;
}

struct CPU_CAPS
{
	bool SSE;
	bool SSE2;
	bool SSE41;
};

inline CPU_CAPS g_cpu{};

void InitCPUCaps()
{
	g_cpu.SSE = CpuHasSSE();
	g_cpu.SSE2 = CpuHasSSE2();
	g_cpu.SSE41 = CpuHasSSE41();
}

```


---
มาโครฟังชั้น Std 
---
```c


namespace Fgi::string
{
	 
	int tolower(int c)
	{
		if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
		return c;
	}
	int stricmp(const char* cs, const char* ct)
	{
		if (cs && ct)
		{
			while (tolower(*cs) == tolower(*ct))
			{
				if (*cs == 0 && *ct == 0) return 0;
				if (*cs == 0 || *ct == 0) break;
				cs++;
				ct++;
			}
			return tolower(*cs) - tolower(*ct);
		}
		return -1;
	}


	int strcmp(const char* cs, const char* ct)
	{
		if (cs && ct)
		{
			while (*cs == *ct)
			{
				if (*cs == 0 && *ct == 0) return 0;
				if (*cs == 0 || *ct == 0) break;
				cs++;
				ct++;
			}
			return *cs - *ct;
		}
		return -1;
	}

	size_t strlen(const char* const string)
	{
		size_t length = 0;

		while (string[length] != '\0')
			length++;

		return length;
	}

	char* strstr(const char* _Str, char const* _SubStr)
	{
		const char* bp = _SubStr;
		const char* back_pos;
		while (*_Str != 0 && _Str != 0 && _SubStr != 0)
		{
			back_pos = _Str;
			while (tolower(*back_pos++) == tolower(*_SubStr++))
			{
				if (*_SubStr == 0)
				{
					return (char*)(back_pos - strlen(bp));
				}
			}
			++_Str;
			_SubStr = bp;
		}
		return 0;
	}
	bool contains_ic(const char* haystack, const char* needle)
	{
		return strstr(haystack, needle) != nullptr;
	}
 
 
	int find(const char* haystack, const char* needle) {

		if (!haystack || !needle) return -1;

		size_t haystackLen = strlen(haystack);
		size_t needleLen = strlen(needle);

		if (needleLen == 0 || needleLen > haystackLen) return -1;

		for (size_t i = 0; i <= haystackLen - needleLen; ++i) {
			bool found = true;
			for (size_t j = 0; j < needleLen; ++j) {
				if (tolower(haystack[i + j]) != tolower(needle[j])) {
					found = false;
					break;
				}
			}
			if (found)
				return (int)i;
		}
		return -1;
	}
	bool starts_with_icase(const char* str, const char* prefix) {
		while (*prefix && *str) {
			if (tolower(*prefix++) != tolower(*str++)) return false;
		}
		return *prefix == 0;
	}
	bool contains_icase(const char* str, const char* sub) {
		size_t lenStr = strlen(str);
		size_t lenSub = strlen(sub);
		for (size_t i = 0; i <= lenStr - lenSub; ++i) {
			if (_strnicmp(&str[i], sub, lenSub) == 0) return true;
		}
		return false;
	}
	// แปลง null-terminated WCHAR* เป็น unsigned long (ฐาน 10)
	unsigned long wstrtoul(const WCHAR* str)
	{
		unsigned long value = 0;
		// ข้าม whitespace เริ่มต้น
		while (*str == L' ' || *str == L'\t' || *str == L'\r' || *str == L'\n')
			++str;

		// อ่านตัวเลข
		while (*str >= L'0' && *str <= L'9') {
			value = value * 10 + (unsigned long)(*str - L'0');
			++str;
		}
		return value;
	}

	// (ถ้าต้องการวัดความยาวของ WCHAR*)
	size_t wstrlen(const WCHAR* str)
	{
		size_t len = 0;
		while (str[len] != L'\0') ++len;
		return len;
	}
	 

	 //Custom is_same implementation using templates
	template<typename T, typename U>
	struct is_same {
		static const bool value = false;
	};

	template<typename T>
	struct is_same<T, T> {
		static const bool value = true;
	};
 
	namespace kernel_string {

		// Return index of first occurrence of substr in str, or -1 if not found
		__forceinline int find(const char* str, size_t len, const char* substr, size_t substr_len, size_t offset = 0)
		{
			if (!str || !substr || substr_len == 0 || len == 0 || substr_len > len || offset >= len) {
				return -1;
			}

			for (size_t i = offset; i <= len - substr_len; ++i) {
				size_t j = 0;
				while (j < substr_len && str[i + j] == substr[j]) {
					++j;
				}

				if (j == substr_len) {
					return static_cast<int>(i); // match found
				}
			}

			return -1; // not found
		}

		// Convenience overloads
		__forceinline int find(const char* str, const char* substr, size_t offset = 0)
		{
			return find(str, strlen(str), substr, strlen(substr), offset);
		}
			// Convenience overloads
		__forceinline int find_skip(const char* str, const char* substr, size_t offset = 0)
		{
			return find(str, strlen(str), substr, strlen(substr), offset);
		}

		__forceinline int find_char(const char* str, char ch, size_t offset = 0)
		{
			if (!str) return -1;
			size_t len = strlen(str);
			for (size_t i = offset; i < len; ++i) {
				if (str[i] == ch) return static_cast<int>(i);
			}
			return -1;
		}
		 
		 
	}
}


// Chack OS_build และอื่นๆ
NTSTATUS GetWindowsDef(
	_Out_ PULONG OutValue1,    // UBR
	_Out_ PULONG OutValue2     // BuildNumber
)
{
	 
	// 2) เตรียม UNICODE_STRING สำหรับ path + value names
	UNICODE_STRING path = RTL_CONSTANT_STRING(
		L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
	);
	UNICODE_STRING nameUBR = RTL_CONSTANT_STRING(L"UBR");
	UNICODE_STRING nameCBN = RTL_CONSTANT_STRING(L"CurrentBuildNumber");
	UNICODE_STRING nameCB = RTL_CONSTANT_STRING(L"CurrentBuild");

	NTSTATUS status;
	HANDLE keyHandle;
	OBJECT_ATTRIBUTES objAttrs;
	ULONG resultLength;
	UCHAR buffer[sizeof(KEY_VALUE_PARTIAL_INFORMATION) + 128] = { 0 };

	InitializeObjectAttributes(
		&objAttrs,
		&path,                                    // ← ต้อง &UNICODE_STRING
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		nullptr,
		nullptr
	);

	// 3) เปิด key
	status = ZwOpenKey(&keyHandle, KEY_READ, &objAttrs);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	// ฟังก์ชันย่อย: อ่าน value เดียว
	auto tryRead = [&](UNICODE_STRING* valueName, PULONG outVal) {
		RtlZeroMemory(buffer, sizeof(buffer));
		status = ZwQueryValueKey(
			keyHandle,
			valueName,                            // ← ส่ง pointer ไปเถอะ
			KeyValuePartialInformation,
			buffer,
			sizeof(buffer),
			&resultLength
		);
		if (!NT_SUCCESS(status)) {
			return status;
		}
		auto info = reinterpret_cast<PKEY_VALUE_PARTIAL_INFORMATION>(buffer);
		if (info->Type == REG_DWORD && info->DataLength == sizeof(ULONG)) {
			*outVal = *reinterpret_cast<PULONG>(info->Data);
		}
		else if (info->Type == REG_SZ && info->DataLength < 128) {
			WCHAR tmp[64] = { 0 };
			RtlCopyMemory(tmp, info->Data, info->DataLength);
			*outVal = Fgi::string::wstrtoul(tmp);
		}
		else {
			return STATUS_INVALID_PARAMETER;
		}
		return STATUS_SUCCESS;
		};

	// 4) อ่าน UBR
	status = tryRead(&nameUBR, OutValue1);
	if (!NT_SUCCESS(status)) {
		ZwClose(keyHandle);
		return status;
	}

	// 5) อ่าน CurrentBuildNumber, ถ้าไม่มี fallback ไป CurrentBuild
	status = tryRead(&nameCBN, OutValue2);
	if (status == STATUS_OBJECT_NAME_NOT_FOUND) {
		status = tryRead(&nameCB, OutValue2);
	}
	ZwClose(keyHandle);
	return status;
}

ULONG ViveWinver()
{

	NTSTATUS st = GetWindowsDef(&MajorViveWinver, &WinVrViveWinver);
	if (WinVrViveWinver == 0) {

		RTL_OSVERSIONINFOW Getver = { 0 };
		Getver.dwOSVersionInfoSize = sizeof(Getver);
		if (Getver.dwBuildNumber == 0) {

			RtlGetVersion(&Getver);

		}

		WinVrViveWinver = Getver.dwBuildNumber;

	}
  switch (WinVrViveWinver)
  {
  case win10_1803:
  case win10_1809:
  	return 0x0278;
  case win10_1903:
  case win10_1909:
  	return 0x0280;
  case win10_2004:
  case win10_20h2:
  case win10_21h1:
  case win10_21h2:
  case win10_22h2:
  	return 0x0388;
  case win11_21h2:
  case win11_22h2:
  	return 0x0390;
  case win11_23h2:
  	return 0x03A0;
  case win11_24h2:
  	return 0x03A0;
  default:
		return 0x03A0;
	}
}

// ฟังชั้น ทำงาน
uintptr_t get_process_cr3(PEPROCESS pprocess)
{
	if (!pprocess) return 0;
	uintptr_t process_dirbase = *(uintptr_t*)((UINT8*)pprocess + 0x28);
	if (process_dirbase == 0)
	{
		ULONG user_diroffset = ViveWinver();
		process_dirbase = *(uintptr_t*)((UINT8*)pprocess + user_diroffset);
	}
	if ((process_dirbase >> 0x38) == 0x40)
	{
  		if (saved_dirbase) return saved_dirbase;
	}
	return process_dirbase;
}
```



