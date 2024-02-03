#include "anya_hook.hpp"
#include "hde/hde32_disasm.hpp"

#include <Windows.h>

Hook::Hook()
    : function_o(nullptr), function_t(nullptr), function_size(0) {}

void Hook::set_memory_protect(uintptr_t address, size_t size, DWORD protect) {
    DWORD old_protect;
    VirtualProtect(reinterpret_cast<void*>(address), size, protect, &old_protect);
}

void Hook::detour(const uintptr_t to_hook, const uintptr_t to_replace, const size_t length) {
    set_memory_protect(to_hook, length, PAGE_EXECUTE_READWRITE);

    function_o = reinterpret_cast<uint8_t*>(std::malloc(length));

    std::memcpy(function_o, reinterpret_cast<void*>(to_hook), length);
    std::memset(reinterpret_cast<void*>(to_hook), 0x90, length);

    *reinterpret_cast<uint8_t*>(to_hook) = 0xE9;
    *reinterpret_cast<uintptr_t*>(to_hook + 1) = (to_replace - to_hook - 5);

    set_memory_protect(to_hook, length, PAGE_EXECUTE_READ);
}

uintptr_t Hook::hook(const uintptr_t to_hook, const uintptr_t to_replace, int32_t instr_nops) {
    uintptr_t at = to_hook;
    function_size = 0;

    for (uintptr_t at = to_hook; ;) {
        hde32s disasm{ 0 };
        hde32_disasm(reinterpret_cast<void*>(at), &disasm);

        // ty jono mwah for pointing this out
        // check for available space *after* incrementing function_size
        // old code checked function_size before incrementing
        if (function_size + disasm.len > 5) {
            if (instr_nops) {
                instr_nops--;
            }
            else {
                break;
            }
        }

        at += disasm.len;
        function_size += disasm.len;
    }

    const uintptr_t detour_address = reinterpret_cast<uintptr_t>
        (VirtualAlloc(nullptr, function_size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    std::memmove(reinterpret_cast<void*>(detour_address), reinterpret_cast<void*>(to_hook), function_size);

    *reinterpret_cast<uint8_t*>(detour_address + function_size) = 0xE9;

    const uintptr_t rel_addr = (to_hook - detour_address - 5);
    *reinterpret_cast<uint32_t*>(detour_address + function_size + 1) = rel_addr;

    detour(to_hook, to_replace, function_size);

    // change back to PAGE_EXECUTE_READ after patching
    set_memory_protect(detour_address, function_size + 5, PAGE_EXECUTE_READ);

    return detour_address;
}

void Hook::unhook(uintptr_t to_unhook) {
    set_memory_protect(to_unhook, function_size + 5, PAGE_EXECUTE_READWRITE);
    std::memcpy(reinterpret_cast<void*>(to_unhook), function_o, function_size);
    set_memory_protect(to_unhook, function_size + 5, PAGE_EXECUTE_READ);

    VirtualFree(reinterpret_cast<void*>(to_unhook), 0, MEM_FREE);

    std::free(function_o);
    to_unhook = 0;
}

void Hook::pause(uintptr_t to_pause) {
    set_memory_protect(to_pause, function_size, PAGE_EXECUTE_READWRITE);
    function_t = reinterpret_cast<uint8_t*>
        (VirtualAlloc(nullptr, function_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    std::memcpy(function_t, reinterpret_cast<void*>(to_pause), function_size);
    std::memcpy(reinterpret_cast<void*>(to_pause), function_o, function_size);

    set_memory_protect(to_pause, function_size, PAGE_EXECUTE_READ);
}

void Hook::resume(uintptr_t to_resume) {
    set_memory_protect(to_resume, function_size, PAGE_EXECUTE_READWRITE);
    std::memcpy(reinterpret_cast<void*>(to_resume), function_t, function_size);
    set_memory_protect(to_resume, function_size, PAGE_EXECUTE_READ);

    VirtualFree(function_t, 0, MEM_RELEASE);
}
