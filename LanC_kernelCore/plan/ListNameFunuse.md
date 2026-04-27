---
ย้ายการทำงานจากUsermode ฟังชั้นที่ อยู่ในกลุ่มต้องสงสัยให้kernel ทำแทน 
---
//RtlQueryModuleInformation   IN Usermode  {#define GetModuleInformation        K32GetModuleInformation}

#define EnumProcesses               K32EnumProcesses
#define GetProcessMemoryInfo        K32GetProcessMemoryInfo
#define GetModuleInformation        K32GetModuleInformation
#define GetModuleBaseNameA          K32GetModuleBaseNameA
#define GetModuleBaseNameW          K32GetModuleBaseNameW
#define GetModuleFileNameExA        K32GetModuleFileNameExA
#define GetModuleFileNameExW        K32GetModuleFileNameExW
#define QueryWorkingSet             K32QueryWorkingSet
#define QueryWorkingSetEx           K32QueryWorkingSetEx

 

```cpp


[0x48,0x89,0x5C,0x24,0x10,0x48,0x89,-]> [2436] 0xFFFFF804119CB8E0|Size:0x19 [Off:Rva:0x7CB8E0|0xFFFFD5609BB5B8E0] (): RtlQueryModuleInformation

// RtlQueryModuleInformation only accepts record sizes 0x8 and 0x110.
// UnitSize is treated as an unsigned selector, and the ModuleInformation
// buffer layout depends on the selected record size.
NTSYSAPI
NTSTATUS
NTAPI
RtlQueryModuleInformation(
    _Inout_ PULONG BufferSize,
    _In_ ULONG UnitSize, // RTL_QUERY_MODULE_INFORMATION_RECORD_SIZE_*
    _Out_writes_bytes_opt_(*BufferSize) PVOID ModuleInformation
    );


NTKERNELAPI
BOOLEAN
NTAPI
ExEnumHandleTable(
    _In_ PHANDLE_TABLE HandleTable,
    _In_ PEX_ENUM_HANDLE_CALLBACK EnumHandleProcedure,
    _Inout_ PVOID Context,
    _Out_opt_ PHANDLE Handle
    );


```






---
Baise inject 
ตรวจสอบตำแหน่องและ หาโครงสร้าง 
--- 
 
```c

/**
** **Filename "ntoskrnl.exe"
*******Section Info
*:
*  [.rdata] 0x1000 - 0xFFFFF80411201000 (Size: 0xCF3A0)
*  [.pdata] 0xD1000 - 0xFFFFF804112D1000 (Size: 0x71F58)
*  [.idata] 0x143000 - 0xFFFFF80411343000 (Size: 0x21EC)
*  [.edata] 0x146000 - 0xFFFFF80411346000 (Size: 0x1A78F)
*  [PROTDATA] 0x161000 - 0xFFFFF80411361000 (Size: 0x1)
*  [GFIDS] 0x162000 - 0xFFFFF80411362000 (Size: 0xA210)
*  [Pad1] 0x16D000 - 0xFFFFF8041136D000 (Size: 0x93000)
*  [.text] 0x200000 - 0xFFFFF80411400000 (Size: 0x47D6BD)
*  [PAGE] 0x67E000 - 0xFFFFF8041187E000 (Size: 0x408316)
*  [PAGELK] 0xA87000 - 0xFFFFF80411C87000 (Size: 0x26F34)
*  [POOLCODE´] 0xAAE000 - 0xFFFFF80411CAE000 (Size: 0x10B4)
*  [PAGEKD] 0xAB0000 - 0xFFFFF80411CB0000 (Size: 0x5EF8)
*  [PAGEVRFYû'] 0xAB6000 - 0xFFFFF80411CB6000 (Size: 0x327FB)
*  [PAGEHDLS'] 0xAE9000 - 0xFFFFF80411CE9000 (Size: 0x2703)
*  [PAGEBGFXýj] 0xAEC000 - 0xFFFFF80411CEC000 (Size: 0x6AFD)
*  [TRACESUP] 0xAF3000 - 0xFFFFF80411CF3000 (Size: 0x1903)
*  [PAGECMRCW] 0xAF5000 - 0xFFFFF80411CF5000 (Size: 0xF57)
*  [KVASCODE=$] 0xAF6000 - 0xFFFFF80411CF6000 (Size: 0x243D)
*  [RETPOL] 0xAF9000 - 0xFFFFF80411CF9000 (Size: 0x760)
*  [INITKDBG‚™] 0xAFA000 - 0xFFFFF80411CFA000 (Size: 0x19982)
*  [MINIEX] 0xB14000 - 0xFFFFF80411D14000 (Size: 0x25AE)
*  [INIT] 0xB17000 - 0xFFFFF80411D17000 (Size: 0x95C1C)
*  [Pad2] 0xBAD000 - 0xFFFFF80411DAD000 (Size: 0x53000)
*  [.data] 0xC00000 - 0xFFFFF80411E00000 (Size: 0x11C1C8)
*  [ALMOSTRO°¹] 0xD1D000 - 0xFFFFF80411F1D000 (Size: 0x2B9B0)
*  [CACHEALI@] 0xD49000 - 0xFFFFF80411F49000 (Size: 0x9040)
*  [PAGEDATA 1] 0xD53000 - 0xFFFFF80411F53000 (Size: 0x13120)
*  [PAGEVRFDP–] 0xD67000 - 0xFFFFF80411F67000 (Size: 0x19650)
*  [INITDATA´Ç] 0xD81000 - 0xFFFFF80411F81000 (Size: 0x1C7B4)
 * [Pad3] 0xD9E000 - 0xFFFFF80411F9E000 (Size: 0x62000)
 * [CFGRO] 0xE00000 - 0xFFFFF80412000000 (Size: 0x2030)
 * [Pad4] 0xE03000 - 0xFFFFF80412003000 (Size: 0x1FD000)
 * [.rsrc] 0x1000000 - 0xFFFFF80412200000 (Size: 0x3BD4C)
 * [.reloc] 0x103C000 - 0xFFFFF8041223C000 (Size: 0xA5E4)
*[+] Export Directory RVA: 0x146000 → Address: 0xFFFFF80411346000 | NumberOfNames: 3264
*
*[🔧] Exported Functions (3264 total):
*
*[0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,-]> [2824] 0xFFFFF804116141B0|Size:0x17 [Off:Rva:0x4141B0|0xFFFFD5609B7A41B0] ZwAllocateVirtualMemory
*[0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,-]> [2825] 0xFFFFF80411614D70|Size:0x19 [Off:Rva:0x414D70|0xFFFFD5609B7A4D70] ZwAllocateVirtualMemoryEx
*
*
*in rip pointer IAT 
*	Line  2731: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [238] IAT Addr: 0xFFFFF802520B48D8 TRv: 0x3B48D8 → ZwAllocateVirtualMemory (FuncPtr: 0xFFFFF802BC674940) Size: 0x17
*	Line  6925: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [115] IAT Addr: 0xFFFFF80251C85678 TRv: 0x2D5678 → ZwAllocateVirtualMemory (FuncPtr: 0xFFFFF802BC674940) Size: 0x17
*	Line 10103: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [221] IAT Addr: 0xFFFFD5609AEB1830 TRv: 0x381830 → ZwAllocateVirtualMemory (FuncPtr: 0xFFFFF804116141B0) Size: 0x17
*	Line 14540: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [128] IAT Addr: 0xFFFFD5609AAB95F8 TRv: 0x2B95F8 → ZwAllocateVirtualMemory (FuncPtr: 0xFFFFF804116141B0) Size: 0x17
*	Line 15462: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [208] IAT Addr: 0xFFFFF80414BF06F8 TRv: 0x1606F8 → ZwAllocateVirtualMemory (FuncPtr: 0xFFFFF804116141B0) Size: 0x17
*	Line 15511: [,0x48,0x8B,0xC4,0xFA,0x48,0x83,0xEC,0x10]| [257] IAT Addr: 0xFFFFF80414BF0880 TRv: 0x160880 → ZwAllocateVirtualMemoryEx (FuncPtr: 0xFFFFF80411614D70) Size: 0x19
*
 */

_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
ZwAllocateVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID *BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG AllocationType,
    _In_ ULONG PageProtection
    );


/**
 * The NtAllocateVirtualMemory routine reserves, commits, or both, a region of pages within the user-mode virtual address space of a specified process.
 *
 * \param ProcessHandle A handle for the process for which the mapping should be done.
 * \param BaseAddress A pointer to a variable that will receive the base address of the allocated region of pages.
 * If the initial value is not zero, the region is allocated at the specified virtual address.
 * \param ZeroBits The number of high-order address bits that must be zero in the base address of the section view.
 * This value must be less than 21 and the initial value of BaseAddress must be zero.
 * \param RegionSize A pointer to a variable that will receive the actual size, in bytes, of the allocated region of pages.
 * \param AllocationType A bitmask containing flags that specify the type of allocation to be performed.
 * \param PageProtection A bitmask containing page protection flags that specify the protection desired for the committed region of pages.
 * \return NTSTATUS Successful or errant status.
 * \sa https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-zwallocatevirtualmemory
 */


_Must_inspect_result_
_When_(return == 0, __drv_allocatesMem(mem))
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
NtAllocateVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID *BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG AllocationType,
    _In_ ULONG PageProtection
    );


/**
 * The NtProtectVirtualMemory routine changes the protection on a region of virtual memory.
 *
 * \param ProcessHandle A handle to the process whose memory protection is to be changed.
 * \param BaseAddress A pointer to the base address of the region of pages whose access protection attributes are to be changed.
 * \param RegionSize A pointer to a variable that specifies the size of the region whose access protection attributes are to be changed.
 * \param NewProtection The memory protection option. This parameter can be one of the memory protection constants.
 * \param OldProtection A pointer to a variable that receives the previous access protection of the first page in the specified region of pages.
 * \return NTSTATUS Successful or errant status.
 */
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
NtProtectVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID *BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG NewProtection,
    _Out_ PULONG OldProtection
    );
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID *BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG NewProtection,
    _Out_ PULONG OldProtection
    );


_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID *BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG NewProtection,
    _Out_ PULONG OldProtection
    );

 
```
