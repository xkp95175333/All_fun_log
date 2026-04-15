
---
mainloop
---
```cpp

	case SuspendTarget_Process:
	{
		//Engine::aip::SuspendTargetProcess((HANDLE)args->processID);
		NTSTATUS GetSusp = OffAutoSuspendProAttach((HANDLE)args->processID);
		if (!NT_SUCCESS(GetSusp)) {
			//	logChack("UN_SUCCESS SuspendTargetProcess: 0x%X\n", GetSusp);
			break;
		}

		break;
	}
	case Resume_Process:
	{
		//	Engine::aip::ResumeTargetProcess((HANDLE)args->processID);
		NTSTATUS Std_Resume = OffAutoResumeProAttach((HANDLE)args->processID);

		if (!NT_SUCCESS(Std_Resume))
		{
			//logChack("!!!Failed ResumeProcess pid:0x%d Status: 0x%X\n", args->processID, Std_Resume);
			break;

		}
		break;
	}

```


---
ฟังชั้นทำงาน
---
```cpp

	NTSTATUS PsResumeProcess(PEPROCESS Process);
  NTSTATUS PsSuspendProcess(PEPROCESS Process);
//

class AutoAttachOffset {
private:
	KAPC_STATE apc_state = {};
	PEPROCESS process = NULL ;
	 
public:
	AutoAttachOffset(PEPROCESS targetProcess) : process(targetProcess) {

		if (process) {
			KeStackAttachProcess(process, &apc_state);
		 
		}
	}

	~AutoAttachOffset() {
		if (process) {
			 
			KeUnstackDetachProcess(&apc_state);
		}
	}
};

NTSTATUS OffAutoSuspendProAttach(HANDLE processID) {
	PEPROCESS targetProcess = nullptr;
	NTSTATUS 	status = PsLookupProcessByProcessId((HANDLE)processID, &targetProcess);
	if (!NT_SUCCESS(status)) {
		logChack("PsLookupProcessByProcessId failed with status: 0x%X\n", status);
		return status;
	}
 
	status = STATUS_UNSUCCESSFUL;
	status = PsSuspendProcess(targetProcess);
	if (!NT_SUCCESS(status)) {
		//ถ้ายังหยุดไม่ได้่
		logChack("[Base]Failed to suspend process: 0x%X\n", status);
		status = STATUS_UNSUCCESSFUL;
		 

		AutoAttachOffset attach(targetProcess); // Attach โดยอัตโนมัติ

		// ทำงานของคุณ
		status = PsSuspendProcess(targetProcess);
		if (!NT_SUCCESS(status)) {
			logChack("[attach]Failed to suspend process: 0x%X\n", status);
		}


	}

	// ล้างค่า
	if (targetProcess) {
		ObDereferenceObject(targetProcess);
		targetProcess = nullptr;
	}

	return status;
}

NTSTATUS OffAutoResumeProAttach(HANDLE processID) {
	PEPROCESS targetProcess = nullptr;
	NTSTATUS 	status = PsLookupProcessByProcessId((HANDLE)processID, &targetProcess);
	if (!NT_SUCCESS(status)) {
		logChack("PsLookupProcessByProcessId failed with status: 0x%X\n", status);
		return status;
	}
	//logChack("[Base]successfully GetProcesss: PID  %d\n", ProcessID);
	//logChack("[Base]successfully GetProcesss: PEPROCESS  %d\n", targetProcess);
	status = STATUS_UNSUCCESSFUL;
	status = PsResumeProcess(targetProcess);
	if (!NT_SUCCESS(status)) {
		//ถ้ายังหยุดไม่ได้่
		logChack("[Base]Failed to Resume process: 0x%X\n", status);
		status = STATUS_UNSUCCESSFUL;
		//logChack("PsResumeProcess failed with status: 0x%X\n", status);
		//logChack("GetAttachProcess  \n");


		AutoAttachOffset attach(targetProcess); // Attach โดยอัตโนมัติ

		// ทำงานของคุณ
		status = PsResumeProcess(targetProcess);
		if (!NT_SUCCESS(status)) {
			logChack("[attach]Failed to Resume process: 0x%X\n", status);
		}


	} // AutoDetach จะถูกเรียกเมื่อออกจาก scope

	// ล้างค่า
	if (targetProcess) {
		ObDereferenceObject(targetProcess);
		targetProcess = nullptr;
	}

	return status;
}
```
