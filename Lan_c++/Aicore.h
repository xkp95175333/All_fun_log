# EzPDBRuntime.h

```cpp
#pragma once

// ============================================================================
// EzPDBRuntime.h
// Single Header Runtime PDB + LLVM Demangle + Capstone Integration
// ============================================================================
// Designed for:
//  - Runtime symbol resolving
//  - PDB parsing
//  - Function analysis
//  - .text scanning
//  - Demangling C++ names
//  - Capstone disassembly
//  - Variable / parameter metadata extraction
//  - Reverse engineering helper runtime
//
// Suggested libs:
//  - LLVMDemangle.lib
//  - Capstone.lib
//  - Pdb.lib / DIA SDK
//
// Target:
//  - C++17+
//  - Windows x64
// ============================================================================

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <dia2.h>
#include <comdef.h>

#include <capstone/capstone.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <algorithm>

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Ole32.lib")

// ============================================================================
// LLVM DEMANGLE
// ============================================================================
// You can replace this include with your local path.
// Example:
// #include <llvm/Demangle/Demangle.h>
//
// Then link:
// LLVMDemangle.lib
// ============================================================================

namespace EzPDB
{

    // ========================================================================
    // Utility Structures
    // ========================================================================

    struct SectionInfo
    {
        std::string Name;
        uintptr_t  Start = 0;
        uintptr_t  End = 0;
        uint32_t   Size = 0;
    };

    struct Instruction
    {
        uintptr_t Address = 0;
        std::string Mnemonic;
        std::string Operand;
        std::vector<uint8_t> Bytes;
    };

    struct FunctionParameter
    {
        std::string Name;
        std::string Type;
        uint32_t StackOffset = 0;
        uint32_t RegisterId = 0;
        uint32_t Size = 0;
    };

    struct LocalVariable
    {
        std::string Name;
        std::string Type;
        uint32_t Offset = 0;
        uint32_t Size = 0;
    };

    struct FunctionInfo
    {
        std::string Name;
        std::string DemangledName;

        uintptr_t StartAddress = 0;
        uintptr_t EndAddress = 0;

        uint32_t Size = 0;

        std::vector<FunctionParameter> Parameters;
        std::vector<LocalVariable>     Locals;
        std::vector<Instruction>       Instructions;
    };

    struct TypeInfo
    {
        std::string Name;
        uint32_t Size = 0;

        std::vector<std::string> Fields;
    };

    // ========================================================================
    // Memory Scanner
    // ========================================================================

    class MemoryScanner
    {
    public:

        static std::vector<uintptr_t> FindPattern(
            uintptr_t start,
            size_t size,
            const std::string& pattern,
            const std::string& mask)
        {
            std::vector<uintptr_t> results;

            auto data = reinterpret_cast<uint8_t*>(start);

            for (size_t i = 0; i < size; i++)
            {
                bool found = true;

                for (size_t j = 0; j < mask.size(); j++)
                {
                    if (mask[j] == 'x' &&
                        pattern[j] != *(char*)(data + i + j))
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                    results.push_back(start + i);
            }

            return results;
        }
    };

    // ========================================================================
    // LLVM Demangle Wrapper
    // ========================================================================

    class Demangler
    {
    public:

        static std::string Demangle(const std::string& name)
        {
            // =================================================================
            // Replace this section with:
            // return llvm::demangle(name);
            // =================================================================

            char buffer[4096]{};

            if (UnDecorateSymbolName(
                name.c_str(),
                buffer,
                sizeof(buffer),
                UNDNAME_COMPLETE))
            {
                return buffer;
            }

            return name;
        }

        static bool IsCppSymbol(const std::string& name)
        {
            return name.find("??") != std::string::npos ||
                   name.find("::") != std::string::npos;
        }
    };

    // ========================================================================
    // Capstone Runtime
    // ========================================================================

    class CapstoneRuntime
    {
    private:

        csh m_Handle = 0;

    public:

        bool Initialize()
        {
            if (cs_open(CS_ARCH_X86, CS_MODE_64, &m_Handle) != CS_ERR_OK)
                return false;

            cs_option(m_Handle, CS_OPT_DETAIL, CS_OPT_ON);

            return true;
        }

        void Shutdown()
        {
            if (m_Handle)
                cs_close(&m_Handle);
        }

        std::vector<Instruction> Disassemble(
            uintptr_t address,
            const uint8_t* code,
            size_t size)
        {
            std::vector<Instruction> result;

            cs_insn* insn = nullptr;

            size_t count = cs_disasm(
                m_Handle,
                code,
                size,
                address,
                0,
                &insn);

            for (size_t i = 0; i < count; i++)
            {
                Instruction inst;

                inst.Address = insn[i].address;
                inst.Mnemonic = insn[i].mnemonic;
                inst.Operand = insn[i].op_str;

                inst.Bytes.assign(
                    insn[i].bytes,
                    insn[i].bytes + insn[i].size);

                result.push_back(inst);
            }

            cs_free(insn, count);

            return result;
        }
    };

    // ========================================================================
    // PE Runtime
    // ========================================================================

    class PERuntime
    {
    private:

        HMODULE m_Module = nullptr;

    public:

        bool Attach(HMODULE mod)
        {
            m_Module = mod;
            return m_Module != nullptr;
        }

        uintptr_t GetBase() const
        {
            return reinterpret_cast<uintptr_t>(m_Module);
        }

        IMAGE_NT_HEADERS* GetNtHeader()
        {
            auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(m_Module);

            return reinterpret_cast<IMAGE_NT_HEADERS*>(
                reinterpret_cast<uint8_t*>(m_Module) + dos->e_lfanew);
        }

        std::vector<SectionInfo> GetSections()
        {
            std::vector<SectionInfo> sections;

            auto nt = GetNtHeader();

            auto sec = IMAGE_FIRST_SECTION(nt);

            for (size_t i = 0; i < nt->FileHeader.NumberOfSections; i++)
            {
                SectionInfo info;

                info.Name = reinterpret_cast<char*>(sec[i].Name);
                info.Start = GetBase() + sec[i].VirtualAddress;
                info.Size = sec[i].Misc.VirtualSize;
                info.End = info.Start + info.Size;

                sections.push_back(info);
            }

            return sections;
        }

        std::optional<SectionInfo> GetTextSection()
        {
            auto sections = GetSections();

            for (auto& s : sections)
            {
                if (s.Name == ".text")
                    return s;
            }

            return std::nullopt;
        }
    };

    // ========================================================================
    // DIA PDB Runtime
    // ========================================================================

    class PDBRuntime
    {
    private:

        IDiaDataSource*     m_Source = nullptr;
        IDiaSession*        m_Session = nullptr;
        IDiaSymbol*         m_Global = nullptr;

        std::wstring        m_PdbPath;

    public:

        ~PDBRuntime()
        {
            Shutdown();
        }

        bool Initialize(const std::wstring& pdbPath)
        {
            m_PdbPath = pdbPath;

            CoInitialize(nullptr);

            HRESULT hr = CoCreateInstance(
                __uuidof(DiaSource),
                nullptr,
                CLSCTX_INPROC_SERVER,
                __uuidof(IDiaDataSource),
                (void**)&m_Source);

            if (FAILED(hr))
                return false;

            hr = m_Source->loadDataFromPdb(m_PdbPath.c_str());

            if (FAILED(hr))
                return false;

            hr = m_Source->openSession(&m_Session);

            if (FAILED(hr))
                return false;

            hr = m_Session->get_globalScope(&m_Global);

            return SUCCEEDED(hr);
        }

        void Shutdown()
        {
            if (m_Global)
            {
                m_Global->Release();
                m_Global = nullptr;
            }

            if (m_Session)
            {
                m_Session->Release();
                m_Session = nullptr;
            }

            if (m_Source)
            {
                m_Source->Release();
                m_Source = nullptr;
            }

            CoUninitialize();
        }

        std::vector<FunctionInfo> GetFunctions()
        {
            std::vector<FunctionInfo> list;

            if (!m_Global)
                return list;

            IDiaEnumSymbols* enumSymbols = nullptr;

            if (FAILED(m_Global->findChildren(
                SymTagFunction,
                nullptr,
                nsNone,
                &enumSymbols)))
            {
                return list;
            }

            IDiaSymbol* symbol = nullptr;
            ULONG fetched = 0;

            while (SUCCEEDED(enumSymbols->Next(1, &symbol, &fetched)) && fetched)
            {
                BSTR name;

                if (SUCCEEDED(symbol->get_name(&name)))
                {
                    FunctionInfo info;

                    std::wstring ws(name);

                    info.Name = std::string(ws.begin(), ws.end());
                    info.DemangledName = Demangler::Demangle(info.Name);

                    ULONGLONG va = 0;
                    DWORD len = 0;

                    symbol->get_virtualAddress(&va);
                    symbol->get_length(&len);

                    info.StartAddress = static_cast<uintptr_t>(va);
                    info.Size = len;
                    info.EndAddress = info.StartAddress + len;

                    list.push_back(info);
                }

                symbol->Release();
            }

            enumSymbols->Release();

            return list;
        }

        std::optional<FunctionInfo> FindFunction(
            const std::string& name)
        {
            auto funcs = GetFunctions();

            for (auto& fn : funcs)
            {
                if (fn.Name == name ||
                    fn.DemangledName == name)
                {
                    return fn;
                }
            }

            return std::nullopt;
        }
    };

    // ========================================================================
    // Main Runtime
    // ========================================================================

    class Runtime
    {
    private:

        PERuntime          m_PE;
        PDBRuntime         m_PDB;
        CapstoneRuntime    m_Capstone;

        std::mutex         m_Mutex;

    public:

        bool Initialize(
            HMODULE module,
            const std::wstring& pdbPath)
        {
            std::scoped_lock lock(m_Mutex);

            if (!m_PE.Attach(module))
                return false;

            if (!m_PDB.Initialize(pdbPath))
                return false;

            if (!m_Capstone.Initialize())
                return false;

            return true;
        }

        void Shutdown()
        {
            m_Capstone.Shutdown();
            m_PDB.Shutdown();
        }

        std::vector<FunctionInfo> BuildFunctionDatabase()
        {
            auto funcs = m_PDB.GetFunctions();

            auto text = m_PE.GetTextSection();

            if (!text)
                return funcs;

            for (auto& fn : funcs)
            {
                if (!fn.StartAddress || !fn.Size)
                    continue;

                auto code = reinterpret_cast<uint8_t*>(fn.StartAddress);

                fn.Instructions = m_Capstone.Disassemble(
                    fn.StartAddress,
                    code,
                    fn.Size);
            }

            return funcs;
        }

        void DumpFunctions()
        {
            auto funcs = BuildFunctionDatabase();

            for (auto& fn : funcs)
            {
                std::cout
                    << "====================================================\n";

                std::cout
                    << "Name      : " << fn.Name << "\n";

                std::cout
                    << "Demangled : " << fn.DemangledName << "\n";

                std::cout
                    << "Address   : 0x"
                    << std::hex
                    << fn.StartAddress
                    << "\n";

                std::cout
                    << "Size      : "
                    << std::dec
                    << fn.Size
                    << "\n\n";

                for (auto& inst : fn.Instructions)
                {
                    std::cout
                        << std::hex
                        << inst.Address
                        << "  "
                        << inst.Mnemonic
                        << "  "
                        << inst.Operand
                        << "\n";
                }

                std::cout << "\n";
            }
        }

        std::vector<uintptr_t> ScanTextPattern(
            const std::string& pattern,
            const std::string& mask)
        {
            auto text = m_PE.GetTextSection();

            if (!text)
                return {};

            return MemoryScanner::FindPattern(
                text->Start,
                text->Size,
                pattern,
                mask);
        }
    };

}

// ============================================================================
// Example Usage
// ============================================================================
/*

#include "EzPDBRuntime.h"

int main()
{
    EzPDB::Runtime runtime;

    runtime.Initialize(
        GetModuleHandleA(nullptr),
        L"test.pdb");

    runtime.DumpFunctions();

    auto result = runtime.ScanTextPattern(
        "\x48\x89\x5C",
        "xxx");

    runtime.Shutdown();
}

*/

// ============================================================================
// Future Expansion Ideas
// ============================================================================
//
// [x] Runtime disassembler
// [x] PDB symbol parser
// [x] LLVM demangler support
// [x] .text section scanner
// [ ] Graph builder
// [ ] CFG builder
// [ ] VM opcode analyzer
// [ ] SIMD register tracking
// [ ] Stack variable tracker
// [ ] Call graph generator
// [ ] Structure rebuilder
// [ ] IDA exporter
// [ ] Cheat Engine exporter
// [ ] JSON SDK exporter
// [ ] ImGui live viewer
// [ ] Auto typedef generator
// [ ] RTTI parser
// [ ] EH parser
// [ ] VTable rebuilder
// [ ] Template analyzer
// [ ] Inline function recovery
// [ ] AVX512 instruction grouping
// [ ] PE relocation analyzer
// [ ] TLS callback parser
// [ ] SEH parser
// [ ] VMProtect helper
// [ ] Themida helper
// ============================================================================

```

ลักษณะนี้จะกลายเป็น runtime framework มากกว่า parser ธรรมดา เพราะมันต่อยอดได้อีกเยอะ:

* Capstone = วิเคราะห์ asm / bytes / flow
* LLVMDemangle = แปลงชื่อ C++ / namespace / template
* PDB = ดึง parameter / type / symbol / class
* PE Runtime = อ่าน .text / section / image
* Runtime class = รวมทุกอย่างเป็น object เดียว

และข้อดีของแบบ single header:

* ย้าย project ง่าย
* inject เข้า tools ได้ง่าย
* compile เร็วเวลา debug runtime tools
* เอาไปทำ SDK generator ได้
* เอาไปต่อกับ ImGui inspector ได้ง่าย
* ทำ live debugger ได้ง่าย

ถ้าจะทำต่อจริง แนะนำเพิ่ม:

* Zydis
* LLVM MC
* DIA Full Symbol Walker
* CFG Graph
* SSA IR
* AST Builder
* Signature Generator
* Auto Struct Recovery
* SIMD State Tracker

ตัวนี้จะเริ่มเข้าแนวเหมือน:

* IDA Runtime
* ReClass Runtime
* Cheat Engine Analyzer
* Ghidra Mini Runtime
* Binary Ninja Style Runtime



//ชุดพัฒนา 2
#include <capstone/capstone.h>
#include <vector>
#include <string>
#include <sstream>

struct Instruction
{
    uintptr_t Address{};
    std::string Mnemonic;
    std::string Operand;
    std::vector<uint8_t> Bytes;

    uintptr_t ResolvedAddress = 0; // [rip+xxx] หลังคำนวณ
};

class CapstoneRuntime
{
private:

    csh m_Handle = 0;

    uintptr_t m_ProcessBase = 0;

public:

    bool Initialize(
        uintptr_t processBase = 0,
        cs_arch customArch = CS_ARCH_X86,
        cs_mode customMode = CS_MODE_64)
    {
        m_ProcessBase = processBase;

        struct TryMode
        {
            cs_arch arch;
            cs_mode mode;
            bool skipData;
        };

        std::vector<TryMode> modes =
        {
            {customArch, customMode,false},

            {CS_ARCH_X86,CS_MODE_64,false},
            {CS_ARCH_X86,CS_MODE_64,true},

            {CS_ARCH_X86,CS_MODE_32,false},
            {CS_ARCH_X86,CS_MODE_32,true},

            {CS_ARCH_X86,CS_MODE_16,false},
        };

        for (auto& m : modes)
        {
            if (m_Handle)
            {
                cs_close(&m_Handle);
                m_Handle = 0;
            }

            if (cs_open(
                m.arch,
                m.mode,
                &m_Handle) != CS_ERR_OK)
            {
                continue;
            }

            cs_option(
                m_Handle,
                CS_OPT_DETAIL,
                CS_OPT_ON);

            if (m.skipData)
            {
                cs_option(
                    m_Handle,
                    CS_OPT_SKIPDATA,
                    CS_OPT_ON);
            }

            return true;
        }

        return false;
    }

    void Shutdown()
    {
        if (m_Handle)
        {
            cs_close(&m_Handle);
            m_Handle = 0;
        }
    }

    std::vector<Instruction> Disassemble(
        uintptr_t address,
        const uint8_t* code,
        size_t size)
    {
        std::vector<Instruction> result;

        if (!m_Handle)
            return result;

        cs_insn* insn = nullptr;

        size_t count =
            cs_disasm(
                m_Handle,
                code,
                size,
                address,
                0,
                &insn);

        for (size_t i = 0; i < count; i++)
        {
            Instruction inst;

            inst.Address =
                insn[i].address;

            inst.Mnemonic =
                insn[i].mnemonic;

            inst.Operand =
                insn[i].op_str;

            inst.Bytes.assign(
                insn[i].bytes,
                insn[i].bytes +
                insn[i].size);

            //-------------------------------------
            // RIP relative resolve
            //-------------------------------------

            if (insn[i].detail)
            {
                auto& x86 =
                    insn[i].detail->x86;

                for (uint8_t j = 0;
                     j < x86.op_count;
                     j++)
                {
                    auto& op =
                        x86.operands[j];

                    if (op.type !=
                        X86_OP_MEM)
                        continue;

                    if (op.mem.base
                        == X86_REG_RIP)
                    {
                        int64_t disp =
                            op.mem.disp;

                        uintptr_t nextInstr =
                            insn[i].address +
                            insn[i].size;

                        uintptr_t realAddr =
                            nextInstr +
                            disp;

                        inst.ResolvedAddress =
                            realAddr;

                        std::stringstream ss;

                        ss
                        << insn[i].op_str
                        << " -> [0x"
                        << std::hex
                        << realAddr
                        << "]";

                        inst.Operand =
                            ss.str();

                        break;
                    }
                }
            }

            result.push_back(
                std::move(inst));
        }

        cs_free(insn,count);

        return result;
    }
};


//ใช้แบบนี้:
CapstoneRuntime cap;

cap.Initialize(
    driver.base_address
);

auto list=
cap.Disassemble(
    address,
    buffer,
    size);

for(auto& x:list)
{
    printf(
        "%llX  %s %s\n",
        x.Address,
        x.Mnemonic.c_str(),
        x.Operand.c_str());

    if(x.ResolvedAddress)
    {
        printf(
            "real: %p\n",
            (void*)x.ResolvedAddress);
    }
}


