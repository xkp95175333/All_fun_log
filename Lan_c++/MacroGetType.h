//ตรวจว่า ใช้ float ไหม 
template<typename T>
bool CheckFloatValue(const T& v) {
	// 1) ต้องเป็น floating-point type
	if constexpr (!std::is_floating_point_v<T>) {
		return false;
	}

	// แปลงเป็น float เพื่อใช้เกณฑ์ที่กำหนด (กรณี T == double จะถูก cast ลง)
	float f = static_cast<float>(v);

	// 2) ตรวจ finite (ไม่ใช่ NaN หรือ ±inf)
	if (!std::isfinite(f)) {
		return false;
	}

	// 3) ตรวจขอบเขตค่าต่ำสุด (abs > epsilon)
	if (!(f > 0.0001f || f < -0.0001f)) {
		return false;
	}

	// 4) ตรวจขอบเขตค่าสูงสุด (abs < limit)
	constexpr float limit = 1'000'000'000.f;
	if (!(f <  limit && f > -limit)) {
		return false;
	}

	return true;
}
//ตรวจว่าใช่ int ไหม 

bool isReasonableInt32(int32_t value) {
	return value > 1 && value < 1000000;
}
//ตรวจว่าเป็นชื่อ ascii ไหม 
bool ChackNameType(const std::string& str) {
	// true คือ อ่านได้ ชื่อ 
	int letterCount = 0;
	int numberCount = 0;

	for (char ch : str) {
		if (std::isalpha(ch)) { // ตรวจสอบตัวอักษร A-Z, a-z
			letterCount++;
		}
		else if (std::isdigit(ch)) { // ตรวจสอบตัวเลข 0-9
			numberCount++;
		}
	}

	return letterCount > numberCount; // คืนค่า true ถ้าตัวอักษรมากกว่าตัวเลข
}
//ตรวจกับฟังชั้น ที่ มี จำนวนอักษรเพื่อ ทำ 100% ไว้  
bool ChackCNameTypeGood(const std::string& str,int16_t Name_len ) {
	//Name_len 80% >= Good
	//
	// true คือ อ่านได้ ชื่อ 
	int letterCount = 0;
	int numberCount = 0;

	for (char ch : str) {
		if (std::isalpha(ch)) { // ตรวจสอบตัวอักษร A-Z, a-z
			letterCount++;
		}
		else if (std::isdigit(ch)) { // ตรวจสอบตัวเลข 0-9
			numberCount++;
		}
	}

	return letterCount > numberCount; // คืนค่า true ถ้าตัวอักษรมากกว่าตัวเลข
}

std::string GuessTypeNameEx8(uintptr_t address, const std::string& nameinClass, bool& trackDuplicates) 
{

	uint64_t BasePackage = ChackAddressBasePackage(address);
	// 1. ตรวจ Disassembly ว่ามี instruction จริงหรือไม่ (Code Pointer)
	uint8_t buf[1024] = {};
	size_t bytesRead = 0;
	uint64_t x86relAdrt = NULL;
	//DECLSPEC_ALIGN(16) DECLSPEC_NOINITALL	CONTEXT ctx = {};
	//if (Proc::ThreadCount == 0)GetModuleMainBase(hProcEx, Proc::Pakinpricess);
	//ctx = GetRuntime_data(address);


	if (useSet->FullTryPtr((PVOID)address, &buf, sizeof(buf), bytesRead) && bytesRead > 0) {
		// 1) ถ้าอ่านได้พอสมควร ให้ disasm ด้วย Capstone
		if (bytesRead >= 16) {
			static bool s_CsInited = false;
			static csh  s_Handle = 0;

			if (!s_CsInited) {


				if (!s_CsInited && cs_open(CS_ARCH_X86, CS_MODE_16, &s_Handle) == CS_ERR_OK
					|| !s_CsInited && cs_open(CS_ARCH_X86, CS_MODE_32, &s_Handle) == CS_ERR_OK
					|| !s_CsInited && cs_open(CS_ARCH_X86, CS_MODE_64, &s_Handle) == CS_ERR_OK

					) {
					cs_option(s_Handle, CS_OPT_LITBASE, CS_OPT_ON);
					//cs_option(s_Handle, CS_OPT_DETAIL, CS_OPT_ON);
					s_CsInited = true;
				}
			}

			if (s_CsInited) {
				cs_insn* insn = nullptr;
				//Chack_Module_engine();

				int count = cs_disasm(s_Handle,
					buf, bytesRead,
					BasePackage, 1, &insn);
				// Inside disassembly block after successful disasm
				if (count > 0) {
					// Check for common code patterns
					bool isLikelyCode = false;
					  x86relAdrt = X86_REL_ADDR(insn[count]);

					// Pattern 1: Function prologue (push rbp/mov rbp,rsp)
					if (count >= 2 &&
						insn[0].id == X86_INS_PUSH &&
						insn[0].detail->x86.operands[0].reg == X86_REG_RBP &&
						insn[1].id == X86_INS_MOV &&
						insn[1].detail->x86.operands[0].reg == X86_REG_RBP &&
						insn[1].detail->x86.operands[1].reg == X86_REG_RSP)
					{
						isLikelyCode = true;
					}
					// Pattern 2: Common instructions
					else if (count >= 1) {
						switch (insn[0].id) {
						case X86_INS_CALL: 
						case X86_INS_RET:
						case X86_INS_JMP:
						case X86_INS_LEA:
							isLikelyCode = true;
							break;
						}
					}

					if (isLikelyCode) {
						cs_free(insn, count);
						trackDuplicates = false;
						return "fun||";
					}

					// Check for vector instructions
					for (int i = 0; i < count; i++) {
						if (cs_insn_group(s_Handle, &insn[i], CS_GRP_INVALID/*CS_GRP_VECTOR*/)) {
							cs_free(insn, count);

							trackDuplicates = false;
							return "vector_instruction||";
						}
					}
				}

			}

			if (s_Handle != 0)  cs_close(&s_Handle);
		}
	}

	// —————————————————————————————————
	// 0. (ใหม่) ตรวจ immediate ASCII ก่อน (ไบต์ภายใน address)
	// —————————————————————————————————
	bool ChackStringAscii = false;
	std::string immAscii = readAscii(address);
	 
	immAscii = TryDemangle(immAscii.data());
	if (!immAscii.empty()) {
		// ถ้านับ alnum/cnt ratio พอใช้
		ChackStringAscii = ChackNameType(immAscii);  // ฟังก์ชันเช็คว่าเป็นชื่อจริงหรือไม่


	} 
	std::wstring ws = readUnicode(address, 556);
	 
	bool ok_s = (!immAscii.empty() && immAscii.size() <= 256);
	bool ok_ws = (!ws.empty() && ws.size() <= 256);

	// ถ้าเป็น wide string ให้ convert to utf8
	std::string ws_as_str;
	if (ok_ws) {
		ws_as_str = WStringToString(ws);
	} 
	ws_as_str = TryDemangle(ws_as_str.data()).c_str();
	// เลือกตัวที่ยาวสุด
	std::string bestName;
	bool isWide = false;
	if (ok_s) {
		bestName = immAscii;
	}
	if (ok_ws) {
		if (!ok_s || ws_as_str.size() > bestName.size()) {
			bestName = ws_as_str;
			isWide = true;
		}
	}

	bool ratio = false;
	// ถ้า bestName ยืดยาวและ ratio alnum >= 0.6 คืนเป็น string/wstring
	if (bestName.size() > 1) {


		ratio = ChackNameType(bestName);  // ฟังก์ชันเช็คว่าเป็นชื่อจริงหรือไม่


	}
	// 3. ตรวจ float/double/int/bool จาก bit-pattern
	float f = *reinterpret_cast<float*>(&address);
	int i32 = *reinterpret_cast<int*>(&address); 
	double d = *reinterpret_cast<double*>(&address);
	if (CheckFloatValue(f)) {
		trackDuplicates = false;
		return "float:" + std::to_string(f) + "||";
	}
	if (std::isfinite(d) && std::fabs(d) > 1e-7 && std::fabs(d) <= 1e12) 
	{
		trackDuplicates = false;
		return "double:" + std::to_string(d) + "||";
	}
	if (isReasonableInt32(i32)) {
		trackDuplicates = false;
		return "int:" + std::to_string(i32) + "||";
	}
	if (i32 == 0 || i32 == 1) {
		trackDuplicates = false;
		return "bool:" + std::to_string(i32) + "||";
	}

	// 4. ตรวจ pointer ที่ไม่ใช่ code
	if (isRegionGood(address))
	{
		std::string finalName = nameinClass;
		if (finalName.empty()) finalName = finalName;

		if (finalName.find("::") != std::string::npos)
			trackDuplicates = false;
		return "Type_UE||";
		if (finalName.find('/') != std::string::npos)
			trackDuplicates = false;
		return "struct||";
		if (finalName.find('.') != std::string::npos)
			trackDuplicates = false;
		return "typedef||";
		if (std::all_of(finalName.begin(), finalName.end(), ::isxdigit))
			trackDuplicates = false;
		return "idkey||";
		if (IsTokenMix(finalName))
			trackDuplicates = false;
		return "tokenkey||";
		if (finalName.size() <= 2 && std::none_of(finalName.begin(), finalName.end(), ::isdigit))
			trackDuplicates = false;
		return "shortkey||";

		// ถ้าทั้งหมดไม่เข้า เงื่อนไข ถือว่าเป็นชื่อ class ธรรมดา
		return "class||";
	}

	// 5. ถ้าเป็น null หรือ pattern พิเศษ
	auto i8 = *reinterpret_cast<int32_t*>(&address);
	auto i82 = *reinterpret_cast<int32_t*>(&address + 4);
	if (i8 == 0 && i82 == 0)trackDuplicates = true; return "None";

	trackDuplicates = false;
	return "Un|";
}






namespace Macro
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

	inline size_t strlen(const char* s) {
		size_t l = 0;
		if (!s) return 0;
		while (s[l] != '\0') l++;
		return l;
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
	 

	uint64_t swap_endian(uint64_t addr) {
		return ((addr & 0xFF) << 56) |
			((addr & 0xFF00) << 40) |
			((addr & 0xFF0000) << 24) |
			((addr & 0xFF000000) << 8) |
			((addr & 0xFF00000000) >> 8) |
			((addr & 0xFF0000000000) >> 24) |
			((addr & 0xFF000000000000) >> 40) |
			((addr & 0xFF00000000000000) >> 56);
	}
	static inline 	uint64_t swap_endianR(uint64_t addr, uint32_t AddrKey) {
		uint64_t GetSize = ((addr & 0xFF) << 56) |
			((addr & 0xFF00) << 40) |
			((addr & 0xFF0000) << 24) |
			((addr & 0xFF000000) << 8) |
			((addr & 0xFF00000000) >> 8) |
			((addr & 0xFF0000000000) >> 24) |
			((addr & 0xFF000000000000) >> 40) |
			((addr & 0xFF00000000000000) >> 56);
		
		return GetSize;
	}
	static inline uint64_t ReverseEndian64(uint64_t addr) {
		return ((addr & 0x00000000000000FFull) << 56) |
			((addr & 0x000000000000FF00ull) << 40) |
			((addr & 0x0000000000FF0000ull) << 24) |
			((addr & 0x00000000FF000000ull) << 8) |
			((addr & 0x000000FF00000000ull) >> 8) |
			((addr & 0x0000FF0000000000ull) >> 24) |
			((addr & 0x00FF000000000000ull) >> 40) |
			((addr & 0xFF00000000000000ull) >> 56);
	}

	static inline uint32_t ReverseEndian32(uint32_t addr) {
		return
			((addr & 0x00000000000000FFull) << 56) |
			((addr & 0x000000000000FF00ull) << 40) |
			((addr & 0x0000000000FF0000ull) << 24) |
			((addr & 0x00000000FF000000ull) << 8); 
	}
	bool HasRepeatingMiddleBytes(uint64_t addr, uint64_t RetrueAptr) {
		uint32_t low32 = (uint32_t)addr;

		// นับว่ากี่ hex digit จริง (ไม่รวม leading zero)
		// เราจะคำนวณ length แบบ manual
		uint32_t tmp = low32;
		int hexLen = 1;
		while (tmp >>= 4) { // shift 4 bits = 1 hex digit
			hexLen++;
		}

		// ดึงออกมาเป็น nibble array [high ... low] เพื่อเช็คกลาง
		// แต่เอาง่ายก่อน: ดูเป็น byte
		uint8_t b0 = (low32 >> 0) & 0xFF;
		uint8_t b1 = (low32 >> 8) & 0xFF;
		uint8_t b2 = (low32 >> 16) & 0xFF;

		if (hexLen <= 6) {
			// ใช้กลาง 2 byte: b1 กับ b2
			if (b1 == b2) return true;
		}
		else {
			// hexLen > 6 (ประมาณ 7-8 หลัก) ใช้กลาง 3 byte: b0,b1,b2
			if (b0 == b1 && b1 == b2) return true;
			if (b0 == b2) return true;
		}

		return false;
	}



	 

	std::string string_To_UTF823(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
		if (nwLen <= 0) return "";

		std::wstring wstr(nwLen, 0);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], nwLen);

		int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (nLen <= 0) return "";

		std::string utf8Str(nLen, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], nLen, nullptr, nullptr);

		return utf8Str;
	}

	std::string WStringToUTF8(const wchar_t* lpwcszWString)
	{
		char* pElementText;
		int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
		::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
		std::string strReturn(pElementText);
		delete[] pElementText;
		return strReturn;
	}

	std::wstring MBytesToWString(const char* lpcszString)
	{
		int len = strlen(lpcszString);
		int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
		wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
		std::wstring wString = (wchar_t*)pUnicode;
		delete[] pUnicode;
		return wString;
	}

	inline std::string WStringToUTF8Ex2(const std::wstring& wstr)
	{
		if (wstr.empty()) return {};

		int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string strTo(size_needed, 0);
		::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, nullptr, nullptr);
		return strTo;
	}

	inline std::string wide_to_utf8(const wchar_t* wbuf, size_t wcharCount)
	{
		if (!wbuf || wcharCount == 0) return {};

		// Stop at first L'\0' to avoid junk past terminator
		size_t realLen = 0;
		while (realLen < wcharCount && wbuf[realLen] != L'\0') {
			realLen++;
		}
		if (realLen == 0) return {};

		// WideCharToMultiByte ต้องการ int
		if (realLen > INT32_MAX) return {};

		int wlen = static_cast<int>(realLen);
		int bytesNeeded = WideCharToMultiByte(
			CP_UTF8,
			0,
			wbuf,
			wlen,
			nullptr,
			0,
			nullptr,
			nullptr
		);
		if (bytesNeeded <= 0) return {};

		std::string out(bytesNeeded, '\0');
		WideCharToMultiByte(
			CP_UTF8,
			0,
			wbuf,
			wlen,
			&out[0],
			bytesNeeded,
			nullptr,
			nullptr
		);
		return out;
	}

	// wide char* -> UTF8
	inline  std::string WCharToUTF8(const wchar_t* ws, size_t wlen) {
		if (!ws || wlen == 0) return {};
		// WideCharToMultiByte ต้องการ int
		if (wlen > INT32_MAX) return {};
		int nW = (int)wlen;
		int bytes = ::WideCharToMultiByte(CP_UTF8, 0,
			ws, nW,
			nullptr, 0,
			nullptr, nullptr);
		if (bytes <= 0) return {};
		std::string out(bytes, '\0');
		::WideCharToMultiByte(CP_UTF8, 0,
			ws, nW,
			out.data(), bytes,
			nullptr, nullptr);
		return out;
	}
	// 1) เช็คว่าเป็น UTF-8 อยู่แล้วหรือเปล่า
	static bool looksUE_utf8(const unsigned char* data, size_t n) {
		size_t i = 0;
		while (i < n) {
			unsigned char c = data[i];
			if (c < 0x80) { i++; continue; }
			else if ((c & 0xE0) == 0xC0) {
				if (i + 1 >= n || (data[i + 1] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
				if (v < 0x80) return false; // overlong
				i += 2;
			}
			else if ((c & 0xF0) == 0xE0) {
				if (i + 2 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x0F) << 12) | ((data[i + 1] & 0x3F) << 6) | (data[i + 2] & 0x3F);
				if (v < 0x800 || (v >= 0xD800 && v <= 0xDFFF)) return false;
				i += 3;
			}
			else if ((c & 0xF8) == 0xF0) {
				if (i + 3 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80 || (data[i + 3] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x07) << 18) | ((data[i + 1] & 0x3F) << 12) |
					((data[i + 2] & 0x3F) << 6) | (data[i + 3] & 0x3F);
				if (v < 0x10000 || v > 0x10FFFF) return false;
				i += 4;
			}
			else return false;
		}
		return true;
	}
	// 2) แปลง "ANSI/ACP" -> UTF-8 (ใช้เฉพาะกรณีจำเป็นจริง ๆ เท่านั้น)
	static std::string ansiUE_to_utf8(const std::string& s, UINT codepage = CP_ACP) {
		if (s.empty()) return {};
		int wlen = MultiByteToWideChar(codepage, 0, s.data(), (int)s.size(), nullptr, 0);
		if (wlen <= 0) return {};
		std::wstring ws(wlen, 0);
		MultiByteToWideChar(codepage, 0, s.data(), (int)s.size(), ws.data(), wlen);

		int u8len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), nullptr, 0, nullptr, nullptr);
		if (u8len <= 0) return {};
		std::string out(u8len, '\0');
		WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), out.data(), u8len, nullptr, nullptr);
		return out;
	}

	// 3) แปลง UTF-16LE -> UTF-8 (สำหรับ wide ของ UE)
	static std::string wideUE_to_utf8(const wchar_t* wbuf, size_t wcharCount) {
		if (!wbuf || wcharCount == 0) return {};
		size_t realLen = 0;
		while (realLen < wcharCount && wbuf[realLen] != L'\0') ++realLen;
		if (realLen == 0) return {};
		int bytes = WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)realLen, nullptr, 0, nullptr, nullptr);
		if (bytes <= 0) return {};
		std::string out(bytes, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)realLen, out.data(), bytes, nullptr, nullptr);
		return out;
	}
	// 4) ทำให้ narrow “เป็น UTF-8 แน่ ๆ” โดยไม่เพี้ยน
	static std::string normalizeUE_narrow_to_utf8(const std::string& in) {
		if (in.empty()) return {};
		if (looksUE_utf8(reinterpret_cast<const unsigned char*>(in.data()), in.size())) {
			return in;                 // มันเป็น UTF-8 อยู่แล้ว -> ห้ามแปลงซ้ำด้วย ACP
		}
		return ansiUE_to_utf8(in);       // ไม่ใช่ UTF-8 -> ค่อยแปลงจาก ACP
	}


	inline   std::string  u16_to_utf8(const char16_t* data, int len) {
		if (!data || len <= 0) return {};
		int bytes = WideCharToMultiByte(CP_UTF8, 0,
			reinterpret_cast<LPCWCH>(data), len,
			nullptr, 0, nullptr, nullptr);
		if (bytes <= 0) return {};
		std::string out(bytes, '\0');
		WideCharToMultiByte(CP_UTF8, 0,
			reinterpret_cast<LPCWCH>(data), len,
			out.data(), bytes, nullptr, nullptr);
		return out;
	}

	static std::string u32_to_utf8(const char32_t* buf32, size_t c32Count)
	{
		if (!buf32 || c32Count == 0) return {};

		// ตัดที่ '\0'
		size_t realLen = 0;
		while (realLen < c32Count && buf32[realLen] != U'\0') {
			realLen++;
		}
		if (realLen == 0) return {};

		// แปลงเป็น UTF-16 (std::u16string)
		std::u16string u16;
		u16.reserve(realLen * 2);

		for (size_t i = 0; i < realLen; ++i) {
			uint32_t cp = static_cast<uint32_t>(buf32[i]);
			if (cp <= 0xFFFF) {
				u16.push_back(static_cast<char16_t>(cp));
			}
			else {
				cp -= 0x10000;
				char16_t hi = static_cast<char16_t>(0xD800 + (cp >> 10));
				char16_t lo = static_cast<char16_t>(0xDC00 + (cp & 0x3FF));
				u16.push_back(hi);
				u16.push_back(lo);
			}
		}

		if (u16.empty()) return {};

		// convert UTF-16LE -> UTF-8 using WideCharToMultiByte
		// note: reinterpret_cast<wchar_t*> assumed wchar_t is UTF-16 on Windows (true)
		const wchar_t* wsrc = reinterpret_cast<const wchar_t*>(u16.data());
		size_t wlen = u16.size();

		return wide_to_utf8(wsrc, wlen);
	}



	// 1) std::wstring -> UTF-8 (คุณมีแล้วชื่อ WStringToUTF8Ex2 / WStringToUTF8)
	//   ผมจะหุ้ม helper ให้สั้นชัดเจน
	static std::string WideToUTF8(const std::wstring& ws)
	{
		if (ws.empty()) return {};

		int bytesNeeded = WideCharToMultiByte(
			CP_UTF8,
			0,
			ws.c_str(),
			(int)ws.size(),
			nullptr,
			0,
			nullptr,
			nullptr
		);
		if (bytesNeeded <= 0) return {};

		std::string out(bytesNeeded, '\0');
		WideCharToMultiByte(
			CP_UTF8,
			0,
			ws.c_str(),
			(int)ws.size(),
			&out[0],
			bytesNeeded,
			nullptr,
			nullptr
		);
		return out;
	}


	// 2) std::u16string -> UTF-8
	//    เราจะแปลงผ่าน WideCharToMultiByte โดย cast buffer เป็น wchar_t*
	//    NOTE: บน Windows wchar_t เป็น UTF-16LE 16-bit => ปลอดภัย
	static std::string U16ToUTF8(const std::u16string& u16)
	{
		if (u16.empty()) return {};

		const wchar_t* wsrc = reinterpret_cast<const wchar_t*>(u16.data());
		size_t wlen = u16.size();

		if (wlen > INT32_MAX) return {};
		int wcount = (int)wlen;

		int bytesNeeded = WideCharToMultiByte(
			CP_UTF8,
			0,
			wsrc,
			wcount,
			nullptr,
			0,
			nullptr,
			nullptr
		);
		if (bytesNeeded <= 0) return {};

		std::string out(bytesNeeded, '\0');
		WideCharToMultiByte(
			CP_UTF8,
			0,
			wsrc,
			wcount,
			&out[0],
			bytesNeeded,
			nullptr,
			nullptr
		);
		return out;
	}


	// 3) std::u32string -> UTF-8
	//    u32 -> u16 surrogate -> UTF-8 (WideCharToMultiByte อีกที)
	static std::string U32ToUTF8(const std::u32string& u32)
	{
		if (u32.empty()) return {};

		std::u16string u16;
		u16.reserve(u32.size() * 2);

		for (char32_t cp32 : u32)
		{
			uint32_t cp = (uint32_t)cp32;
			if (cp <= 0xFFFF)
			{
				u16.push_back((char16_t)cp);
			}
			else
			{
				cp -= 0x10000;
				char16_t hi = (char16_t)(0xD800 + (cp >> 10));
				char16_t lo = (char16_t)(0xDC00 + (cp & 0x3FF));
				u16.push_back(hi);
				u16.push_back(lo);
			}
		}

		return U16ToUTF8(u16);
	}


	// 4) std::u8string -> UTF-8 std::string
	//    ใน C++20 u8string::value_type = char8_t (1 byte) แต่ printf ต้อง char* ธรรมดา
	//    เราสามารถ copy byte-by-byte ได้เหมือน memcpy
	static std::string U8ToUTF8(const std::u8string& u8)
	{
		if (u8.empty()) return {};
		std::string out;
		out.reserve(u8.size());
		for (char8_t c : u8) {
			out.push_back(static_cast<char>(c));
		}
		return out;
	}


	// helper เล็กๆ: นับ printable ascii เพื่อ check ว่าขยะไหม
	static int count_printable_ascii(const std::string& s) {
		int c = 0;
		for (unsigned char ch : s) {
			if (ch >= 0x20 && ch <= 0x7E) c++;
		}
		return c;
	}
	 
	// --- เช็ค UTF-8 เข้มข้น (กันหลุด "읥…") ---
	static bool istext_probably_utf8(const unsigned char* data, size_t n) {
		size_t i = 0;

		while (i < n) {
			unsigned char c = data[i];
			if (c < 0x80) { i++; continue; }
			else if ((c & 0xE0) == 0xC0) {
				if (i + 1 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
				if (v < 0x80) return false; // overlong
				i += 2;
			}
			else if ((c & 0xF0) == 0xE0) {
				if (i + 2 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80) return false;
				if ((data[i + 2] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x0F) << 12) |
					((data[i + 1] & 0x3F) << 6) |
					(data[i + 2] & 0x3F);
				if (v < 0x800) return false;
				if (v >= 0xD800 && v <= 0xDFFF) return false; // surrogate
				i += 3;
			}
			else return false;
		}
		return true;
	}



	// --- ตัดที่ '\0' แรก ---
	 /*  static inline void trim_at_first_null(std::string& s) {
		auto p = s.find('\0');
		if (p != std::string::npos) s.resize(p);
	}*/

	static inline void trim_at_first_null(std::string& s) {
		size_t p = s.find('\0');
		if (p != std::string::npos) s.resize(p);
	}

	// ---------- Helper functions (kept/adjusted) ----------
	static inline void trim_at_first_null_Ex(std::string& s) {
		auto p = s.find("");
		if (p != std::string::npos) s.resize(p);
	}


	static inline size_t shrink_until_nonzero_tail(const uint8_t* p, size_t n) {
		while (n > 0 && p[n - 1] == 0x00) --n;
		return n;
	}

	// is_probably_utf8: เดิมของคุณ (ย้ายมาแบบย่อ)
	static bool is_probably_utf8(const unsigned char* data, size_t n) {
		size_t i = 0;
		while (i < n) {
			unsigned char c = data[i];
			if (c < 0x80) { i++; continue; }
			else if ((c & 0xE0) == 0xC0) {
				if (i + 1 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
				if (v < 0x80) return false;
				i += 2;
			}
			else if ((c & 0xF0) == 0xE0) {
				if (i + 2 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80) return false;
				if ((data[i + 2] & 0xC0) != 0x80) return false;
				unsigned v = ((c & 0x0F) << 12) |
					((data[i + 1] & 0x3F) << 6) |
					(data[i + 2] & 0x3F);
				if (v < 0x800) return false;
				if (v >= 0xD800 && v <= 0xDFFF) return false;
				i += 3;
			}
			else if ((c & 0xF8) == 0xF0) {
				if (i + 3 >= n) return false;
				if ((data[i + 1] & 0xC0) != 0x80) return false;
				if ((data[i + 2] & 0xC0) != 0x80) return false;
				if ((data[i + 3] & 0xC0) != 0x80) return false;
				unsigned v =
					((c & 0x07) << 18) |
					((data[i + 1] & 0x3F) << 12) |
					((data[i + 2] & 0x3F) << 6) |
					(data[i + 3] & 0x3F);
				if (v < 0x10000 || v > 0x10FFFF) return false;
				i += 4;
			}
			else return false;
		}
		return true;
	}


	// UTF-8 validator and decoder (returns codepoint and length)
	static bool decode_utf8_at(const unsigned char* data, size_t n, size_t i, uint32_t& out_cp, int& out_len) {
		unsigned char c = data[i];
		if (c < 0x80) { out_cp = c; out_len = 1; return true; }
		if ((c & 0xE0) == 0xC0) {
			if (i + 1 >= n) return false;
			if ((data[i + 1] & 0xC0) != 0x80) return false;
			out_cp = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
			out_len = 2; return out_cp >= 0x80;
		}
		if ((c & 0xF0) == 0xE0) {
			if (i + 2 >= n) return false;
			if ((data[i + 1] & 0xC0) != 0x80) return false;
			if ((data[i + 2] & 0xC0) != 0x80) return false;
			out_cp = ((c & 0x0F) << 12) | ((data[i + 1] & 0x3F) << 6) | (data[i + 2] & 0x3F);
			out_len = 3; return out_cp >= 0x800 && !(out_cp >= 0xD800 && out_cp <= 0xDFFF);
		}
		if ((c & 0xF8) == 0xF0) {
			if (i + 3 >= n) return false;
			if ((data[i + 1] & 0xC0) != 0x80) return false;
			if ((data[i + 2] & 0xC0) != 0x80) return false;
			if ((data[i + 3] & 0xC0) != 0x80) return false;
			out_cp = ((c & 0x07) << 18) | ((data[i + 1] & 0x3F) << 12) | ((data[i + 2] & 0x3F) << 6) | (data[i + 3] & 0x3F);
			out_len = 4; return out_cp >= 0x10000 && out_cp <= 0x10FFFF;
		}
		return false;
	}

	// Count printable UTF-8 codepoints (stop at maxCount). Printable defined as codepoint >= 0x20 and not 0x7F.
	static int count_printable_utf8_codepoints(const std::string& s, int maxCount) {
		const unsigned char* data = reinterpret_cast<const unsigned char*>(s.data());
		size_t n = s.size();
		size_t i = 0; int counted = 0;
		while (i < n && counted < maxCount) {
			uint32_t cp = 0; int len = 0;
			if (!decode_utf8_at(data, n, i, cp, len)) break;
			if (cp >= 0x20 && cp != 0x7F) ++counted;
			i += len;
		}
		return counted;
	}

	// Flatten UTF-16LE buffer to UTF-8 string (normal conversion); helper uses provided project helpers (u16_to_utf8)
	static std::string flatten_utf16_to_utf8(const uint8_t* p, size_t chars) {
		if (!p || chars == 0) return {};
		auto p16 = reinterpret_cast<const char16_t*>(p);
		// Use project-specific converter; if unavailable, a simple fallback could be implemented.
		return u16_to_utf8(reinterpret_cast<const char16_t*>(p16), static_cast<int>(chars));
	}
 
	static bool Chack_ascii(const std::string& str) {
		int letterCount = 0;
		size_t Sizename_l = str.max_size();
		for (unsigned char ch : str) {
			if (std::isalpha(ch)) { letterCount++; }

		}
		return letterCount;
	} 
			
			
			
	 // ChackNameType as provided (slightly adapted)
	static bool Chack_provided_NameType(const std::string& str) {
		int letterCount = 0;
		int numberCount = 0;
		for (unsigned char ch : str) {
			if (std::isalpha(ch)) { letterCount++; }
			else if (std::isdigit(ch)) { numberCount++; }
		}
		return letterCount > numberCount;
	}



	// ===== ใหม่: ตัวนับคะแนน “แบบคุณ” =====
	// - นับเฉพาะตัวถูกกติกา: [A-Za-z0-9_:.<>]
	// - คืน "จำนวนตัวที่ผ่านเกณฑ์" (int16_t)
	// - expectedLen = nameLength จาก UE (เพื่อใช้ตัดสิน “ครบถ้วน” หรือไม่)
	static int16_t scoreAsciiStyle(const std::string& s, int expectedLen, int& letters, int& numbers)
	{
		letters = 0; numbers = 0;
		int cnt = 0;
		const int N = static_cast<int>(std::min<size_t>(s.size(), (size_t)expectedLen));
		for (int i = 0; i < N; ++i) {
			unsigned char ch = static_cast<unsigned char>(s[i]);
			if ((ch >= 'a' && ch <= 'z') ||
				(ch >= 'A' && ch <= 'Z') ||
				ch == '_' || ch == ':' || ch == '<' || ch == '>' || ch == '.') {
				++letters; ++cnt;
			}
			else if (ch >= '0' && ch <= '9') {
				++numbers; ++cnt;
			}
			else {
				// not counted
			}
		}
		return (int16_t)cnt;
	}


	// ===== Heuristics =====
	static inline bool is_ascii(const std::string& s) {
		for (unsigned char c : s) if (c >= 0x80) return false;
		return true;
	}

	static inline bool has_replacement_char(const std::string& s) {
		return s.find("\xEF\xBF\xBD") != std::string::npos; // '�' in UTF-8
	}

	static inline bool starts_with(std::string_view s, std::string_view p) {
		return s.size() >= p.size() && s.compare(0, p.size(), p) == 0;
	}

	static inline void trim_inplace(std::string& s) {
		auto sp = [](unsigned char c) { return std::isspace(c); };
		auto l = std::find_if_not(s.begin(), s.end(), sp);
		auto r = std::find_if_not(s.rbegin(), s.rend(), sp).base();
		if (l < r) s.assign(l, r); else s.clear();
	}



	// ===== UTF decoder from raw memory (very lightweight) =====
	// พยายามเดา BOM: UTF-8/16/32; ถ้าไม่มี BOM:
	// - ถ้าพบรูปแบบ 0xXX 00 0xYY 00 น่าจะเป็น UTF-16LE
	// - ถ้าพบ 00 0xXX 00 0xYY น่าจะเป็น UTF-16BE
	// - ไม่งั้นตีเป็น UTF-8
	static std::string bytes_to_utf8_guess(const std::vector<unsigned char>& b)
	{
		if (b.empty()) return {};

		// BOM checks
		if (b.size() >= 3 && b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF) {
			return std::string(reinterpret_cast<const char*>(b.data() + 3),
				reinterpret_cast<const char*>(b.data() + b.size()));
		}
		if (b.size() >= 2 && b[0] == 0xFF && b[1] == 0xFE) {
			// UTF-16 LE
			std::u16string u;
			for (size_t i = 2; i + 1 < b.size(); i += 2) u.push_back(char16_t(b[i] | (b[i + 1] << 8)));
			return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u);
		}
		if (b.size() >= 2 && b[0] == 0xFE && b[1] == 0xFF) {
			// UTF-16 BE
			std::u16string u;
			for (size_t i = 2; i + 1 < b.size(); i += 2) u.push_back(char16_t((b[i] << 8) | b[i + 1]));
			return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u);
		}
		if (b.size() >= 4 && b[0] == 0xFF && b[1] == 0xFE && b[2] == 0x00 && b[3] == 0x00) {
			// UTF-32 LE
			std::u32string u;
			for (size_t i = 4; i + 3 < b.size(); i += 4)
				u.push_back(char32_t((uint32_t)b[i] | ((uint32_t)b[i + 1] << 8) | ((uint32_t)b[i + 2] << 16) | ((uint32_t)b[i + 3] << 24)));
			return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(u);
		}
		if (b.size() >= 4 && b[0] == 0x00 && b[1] == 0x00 && b[2] == 0xFE && b[3] == 0xFF) {
			// UTF-32 BE
			std::u32string u;
			for (size_t i = 4; i + 3 < b.size(); i += 4)
				u.push_back(char32_t(((uint32_t)b[i] << 24) | ((uint32_t)b[i + 1] << 16) | ((uint32_t)b[i + 2] << 8) | (uint32_t)b[i + 3]));
			return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(u);
		}

		// No BOM → simple heuristic between UTF-8 / UTF-16
		// ถ้าส่วนใหญ่เป็น pattern LE  (byte1 !=0, byte2==0) → UTF-16LE
		// ถ้าส่วนใหญ่เป็น pattern BE  (byte1==0, byte2!=0) → UTF-16BE
		size_t le_pairs = 0, be_pairs = 0, pairs = 0;
		for (size_t i = 0; i + 1 < b.size(); i += 2) {
			++pairs;
			if (b[i] != 0 && b[i + 1] == 0) ++le_pairs;
			if (b[i] == 0 && b[i + 1] != 0) ++be_pairs;
		}
		if (pairs > 0 && le_pairs * 2 >= pairs) {
			std::u16string u;
			for (size_t i = 0; i + 1 < b.size(); i += 2) u.push_back(char16_t(b[i] | (b[i + 1] << 8)));
			return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u);
		}
		if (pairs > 0 && be_pairs * 2 >= pairs) {
			std::u16string u;
			for (size_t i = 0; i + 1 < b.size(); i += 2) u.push_back(char16_t((b[i] << 8) | b[i + 1]));
			return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u);
		}

		// ตีเป็น UTF-8 ตรงๆ
		return std::string(reinterpret_cast<const char*>(b.data()),
			reinterpret_cast<const char*>(b.data() + b.size()));
	}

	
	static constexpr uint8_t ZeroCountTable[16] = {
		4, // 0x0 → 0000 → 4 zeros
		0, // 0x1
		0, // 0x2
		0, // 0x3
		0, // 0x4
		0, // 0x5
		0, // 0x6
		0, // 0x7
		0, // 0x8
		0, // 0x9
		0, // 0xA
		0, // 0xB
		0, // 0xC
		0, // 0xD
		0, // 0xE
		0  // 0xF
	};
	
	// คืนจำนวน nibble zero จากล่างสุด โดยไม่มีลูป
	 uint32_t CountTrailingZeroNibbles(uint64_t value40)
	{
		// nibble0
		uint8_t n0 = (value40) & 0xF;
		if (n0 != 0) return 0;
	
		// nibble1
		uint8_t n1 = (value40 >> 4) & 0xF;
		if (n1 != 0) return 1;
	
		// nibble2
		uint8_t n2 = (value40 >> 8) & 0xF;
		if (n2 != 0) return 2;
	
		// nibble3
		uint8_t n3 = (value40 >> 12) & 0xF;
		if (n3 != 0) return 3;
	
		// nibble4
		uint8_t n4 = (value40 >> 16) & 0xF;
		if (n4 != 0) return 4;
	
		// nibble5
		uint8_t n5 = (value40 >> 20) & 0xF;
		if (n5 != 0) return 5;
	
		// nibble6
		uint8_t n6 = (value40 >> 24) & 0xF;
		if (n6 != 0) return 6;
	
		// nibble7
		uint8_t n7 = (value40 >> 28) & 0xF;
		if (n7 != 0) return 7;
	
		// nibble8
		uint8_t n8 = (value40 >> 32) & 0xF;
		if (n8 != 0) return 8;
	
		// nibble9
		uint8_t n9 = (value40 >> 36) & 0xF;
		if (n9 != 0) return 9;
	
		return 10;
	}
	
 static inline uint32_t countptr0hot(uint64_t addr)
 {
	 // 1) ตัดเหลือ 10 nibble ล่าง (40 bits)
	 uint64_t v40 = addr & 0xFFFFFFFFFFULL;

	 // 2) หาจำนวน zero-nibble ท้ายสุดแบบไม่มีลูป
	 uint32_t zeros = CountTrailingZeroNibbles(v40);
	 return zeros;

 }
	
	static inline uint64_t TrimLeadingNibb3231q1q2le(uint64_t addr)
	{
		// 1) ตัดเหลือ 10 nibble ล่าง (40 bits)
		uint64_t v40 = addr & 0xFFFFFFFFFFULL;
	
		// 2) หาจำนวน zero-nibble ท้ายสุดแบบไม่มีลูป
		uint32_t zeros = CountTrailingZeroNibbles(v40);
	
		// ถ้าไม่มีการ encode → คืนตามเดิม
		if (zeros == 0)
			return v40;
	
		// 3) ตัด x nibble จากท้าย (shift right)
		uint32_t shiftBits = zeros * 4;
		uint64_t decoded = (v40 >> shiftBits) & 0xFFFFFFFFFFULL;
	
		return decoded;
	}
	

}//enc namespace Macro
