#pragma once
#include "../stdafx.h"

namespace Utils {

	template<typename tai>
	tai read(uintptr_t addr) // tai
	{
		return *(tai*)(addr);
	}

	template <class T>
	inline bool write(uint64_t address, T value)
	{
		*(T*)(address) = value;
		return true;
	}

	static inline bool IsBadMemPtr(bool write, void* ptr, size_t size)
	{
		MEMORY_BASIC_INFORMATION mbi;
		BOOL ok;
		DWORD mask;
		BYTE* p = (BYTE*)ptr;
		BYTE* maxp = p + size;
		BYTE* regend = NULL;
		if (size == 0)
		{
			return FALSE;
		}

		if (p == NULL)
		{
			return TRUE;
		}

		if (write == FALSE)
		{
			mask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
		}
		else
		{
			mask = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
		}

		do
		{
			if (p == ptr || p == regend)
			{
				if (VirtualQuery((LPCVOID)p, &mbi, sizeof(mbi)) == 0)
				{
					return TRUE;
				}
				else
				{
					regend = ((BYTE*)mbi.BaseAddress + mbi.RegionSize);
				}
			}

			ok = (mbi.Protect & mask) != 0;

			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			{
				ok = FALSE;
			}

			if (!ok)
			{
				return TRUE;
			}

			if (maxp <= regend)
			{
				return FALSE;
			}
			else if (maxp > regend)
			{
				p = regend;
			}
		} while (p < maxp);

		return FALSE;
	}

	PIMAGE_NT_HEADERS64 GetHdr(void* img)
	{
		const auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(img);
		if (dos->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;

		const auto nt = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<uintptr_t>(img) + dos->e_lfanew);
		if (nt->Signature != IMAGE_NT_SIGNATURE) return nullptr;

		return nt;
	}

	std::vector<int> PatToBit(const char* patt, char wc)
	{
		auto bytes = std::vector<int>{};
		const auto start = const_cast<char*>(patt);
		const auto end = const_cast<char*>(patt) + strlen(patt);

		for (auto current = start; current < end; ++current)
		{
			if (*current == wc)
			{
				++current;
				if (*current == wc)
					++current;
				bytes.push_back(-1);
			}
			else { bytes.push_back(strtoul(current, &current, 16)); }
		}
		return bytes;
	}

	int count = 1;

	uint64_t FindPattern(void* base, const char* pattern, char wc)
	{
		void* module = 0;
		uint64_t addr = 0;

		module = (void*)base;

		const auto headers = GetHdr(module);

		const auto sizeOfImage = headers->OptionalHeader.SizeOfImage;
		auto patternBytes = PatToBit(pattern, wc);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found) {
				addr = reinterpret_cast<uint64_t>(&scanBytes[i]);
			}
		}
		++count;

		return addr;

	}

	void dbg_print(const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);

		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		vsprintf_s(buffer, sizeof(buffer), ((std::string)xorstr("[momci] ") + msg).c_str(), args);
		spoof(OutputDebugStringA, (LPCSTR)buffer);

		return va_end(args);
	}

	int hk__ = 1;
	void InstallVtableHook(void* addr, void* pDes, int index, void** ret) {

		auto vtable = *(void***)addr;

		if (pDes == read<void*>((uintptr_t)vtable + (index * 0x08)))
			return;

		int methods = 0, i = 0;

		do {
			++methods, ++i;
		} while (read<uintptr_t>((uintptr_t)vtable + (i * 0x8)));

		auto vtable_buf = new uintptr_t[methods * 0x8];

		for (auto count = 0; count < methods; ++count) {
			vtable_buf[count] = read<uintptr_t>((uintptr_t)vtable + (count * 0x8));
		}

		*ret = vtable[index];

		vtable_buf[index] = reinterpret_cast<uintptr_t>(pDes);
		*(uintptr_t**)addr = vtable_buf;

		Utils::dbg_print(xorstr("Hooked %d function!\n"), hk__);
		hk__++;

	}

	PVOID Vtable_function(uintptr_t Object, size_t index)
	{
		auto vtable = *reinterpret_cast<const void***>(Object);

		return const_cast<PVOID>(vtable[index]);
	}

	template<typename T>T Resolve_call(uintptr_t addr) {
		T return_addr = 0;

		if (addr) {
			BYTE byte = *(BYTE*)addr;
			if (byte) {
				if (byte == 0xE9 || byte == 0xE8) { // JMP
					return_addr += *(DWORD*)(addr + 1);
					return_addr += addr + 5;
				}
				else {
					return_addr += *(DWORD*)(return_addr + 3);
					return_addr += addr + 7;
				}
			}
		}

		return return_addr;
	}

	void Free(PVOID offs) {
		static uintptr_t addr = 0;
		if (!addr) { // FMemory::Free
			addr = Utils::FindPattern(Settings::Module, "48 85 C9 74 2E 53", '?');
		}

		auto Free = (__int64(__fastcall*)(__int64))addr;
		spoof(Free, (__int64)offs);
	}

	void cFixName(char* Name)
	{
		for (int i = 0; Name[i] != '\0'; i++)
		{
			if (Name[i] == '_')
			{
				if (Name[i + 1] == '0' ||
					Name[i + 1] == '1' ||
					Name[i + 1] == '2' ||
					Name[i + 1] == '3' ||
					Name[i + 1] == '4' ||
					Name[i + 1] == '5' ||
					Name[i + 1] == '6' ||
					Name[i + 1] == '7' ||
					Name[i + 1] == '8' ||
					Name[i + 1] == '9')
					Name[i] = '\0';
			}
		}

		return;
	}

	std::string GetObjectNameByIndex(uint32_t id) {
		static std::uintptr_t addr = 0;
		if (!addr)
		{
			addr = (Utils::FindPattern(Settings::Module, ("48 89 5C 24 08 57 48 83 EC 30 83 79 04"), '?'));
		}

		auto fn = (FString * (__fastcall*)(std::uint32_t*, FString*))addr;

		FString res;
		fn(&id, &res);

		if (res.c_str() == 0) return "";

		auto tmp = res.ToString();

		char return_string[1024] = {};
		for (size_t i = 0; i < tmp.size(); i++)
		{
			return_string[i] += tmp[i];
		}
		Free((PVOID)res.c_str());

		cFixName(return_string);

		return return_string;
	}

	std::string GetObjectName(uintptr_t Actor) {

		int ID = *(int*)(Actor + 0x18);

		return GetObjectNameByIndex(ID);
	}

	bool CreateHook(uintptr_t originalPresent, uintptr_t hookFunction, uintptr_t pOriginal)
	{
		static uintptr_t addrCreateHook = NULL;

		if (!addrCreateHook)
		{
			addrCreateHook = Utils::FindPattern(Settings::DiscordModule, "41 57 41 56 56 57 55 53 48 83 EC 68 4D 89 C6 49 89 D7", '?');

			printf("CreateHook discord: 0x%x\n", addrCreateHook);
		}

		if (!addrCreateHook)
			return false;

		using CreateHook_t = uint64_t(__fastcall*)(LPVOID, LPVOID, LPVOID*);
		auto fnCreateHook = (CreateHook_t)addrCreateHook;
		return fnCreateHook((void*)originalPresent, (void*)hookFunction, (void**)pOriginal) == 0 ? true : false;

	}

	bool EnableHook(uintptr_t pTarget, bool toggle)
	{
		static uintptr_t addrEnableHook = NULL;

		if (!addrEnableHook)
		{
			addrEnableHook = Utils::FindPattern(Settings::DiscordModule,
				"41 56 56 57 53 48 83 EC 28 49 89 CE BF 01 00 00 00 31 C0 F0 ? ? ? ? ? ? ? 74",
				'?');

			printf("EnableHook discord: 0x%x\n", addrEnableHook);
		}

		if (!addrEnableHook)
			return false;

		using EnableHook_t = uint64_t(__fastcall*)(LPVOID, bool);
		auto fnEnableHook = (EnableHook_t)addrEnableHook;

		return fnEnableHook((void*)pTarget, toggle) == 0 ? true : false;
	}

	bool EnableHookQue()
	{
		static uintptr_t addrEnableHookQueu = NULL;

		if (!addrEnableHookQueu)
		{
			addrEnableHookQueu = Utils::FindPattern(Settings::DiscordModule,
				"41 57 41 56 41 55 41 54 56 57 55 53 48 83 EC 38 48 ? ? ? ? ? ? 48 31 E0 48 89 44 24 30 BE 01 00 00 00 31 C0 F0 ? ? ? ? ? ? ? 74 2B", '?');

		}

		if (!addrEnableHookQueu)
			return false;

		using EnableHookQueu_t = uint64_t(__stdcall*)(VOID);
		auto fnEnableHookQueu = (EnableHookQueu_t)addrEnableHookQueu;

		return fnEnableHookQueu() == 0 ? true : false;
	}

	short GetAsyncKeyState(const int vKey)
	{
		static uintptr_t addrGetAsyncKeyState = NULL;

		if (!addrGetAsyncKeyState)
		{
			addrGetAsyncKeyState = Utils::FindPattern(Settings::DiscordModule,
				"48 FF ? ? ? ? ? CC CC CC CC CC CC CC CC CC 48 FF ? ? ? ? ? CC CC CC CC CC CC CC CC CC 48 83 EC 28 48 ? ? ? ? ? ? 48 85 C9", '?');
		}

		if (!addrGetAsyncKeyState)
			return false;

		using GetAsyncKeyState_t = short(__fastcall*)(int);
		auto fnGetAyncKeyState = (GetAsyncKeyState_t)addrGetAsyncKeyState;

		return fnGetAyncKeyState(vKey);
	}

	short SetCursorPos(int x, int y)
	{
		static uintptr_t addrSetCursorPos = NULL;

		if (!addrSetCursorPos)
		{
			addrSetCursorPos = Utils::FindPattern(Settings::DiscordModule,
				"44 0F B6 05 ? ? ? ? 45 84 C0", '?');
		}

		if (!addrSetCursorPos)
			return false;

		using SetCursorPos_t = short(__fastcall*)(int, int);
		auto fnSetCursorPos = (SetCursorPos_t)addrSetCursorPos;

		return fnSetCursorPos(x, y);
	}

	bool GetCursorPos(LPPOINT lpPoint)
	{
		static uintptr_t addrGetCursorPos = NULL;

		if (!addrGetCursorPos)
		{
			addrGetCursorPos = FindPattern(Settings::DiscordModule,
				"40 53 48 83 EC 20 48 8B D9 FF 15 ? ? ? ? 0F B6 15 ? ? ? ?", '?');

		}

		if (!addrGetCursorPos)
			return false;

		using GetCursorPos_t = short(__fastcall*)(LPPOINT);
		auto fnGetCursorPos = (GetCursorPos_t)addrGetCursorPos;

		return fnGetCursorPos(lpPoint);
	}

	HCURSOR SetCursor(HCURSOR hCursor)
	{
		static uintptr_t addrSetCursor = NULL;

		if (!addrSetCursor)
		{
			addrSetCursor = FindPattern(Settings::DiscordModule,
				"40 53 48 83 EC 20 0F B6 05 ? ? ? ? 48 8B D9 84 C0 74 4F", '?');
		}

		if (!addrSetCursor)
			return 0;

		using SetCursor_t = HCURSOR(__fastcall*)(HCURSOR);
		auto fnSetCursor = (SetCursor_t)addrSetCursor;

		return fnSetCursor(hCursor);
	}

	bool HookFunction(uintptr_t originalFunction, uintptr_t hookedFunction, uintptr_t pOriginalPresent)
	{
		if (CreateHook(originalFunction, hookedFunction, pOriginalPresent))
		{
			if (EnableHook(originalFunction, true))
			{

				if (EnableHookQue())
				{
					return true;
				}
			}
		}

		return false;
	}

}



