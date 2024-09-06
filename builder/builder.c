#include <stdio.h>
#include "c_types.h"
#include "hash_api.h"
#include "errno.h"
#include "runtime.h"
#include "pe_loader.h"
#include "boot.h"
#include "epilogue.h"

// reference from Gleam-RT/include/errno.h 
#define ERR_ARGUMENT_CHECKSUM (0x06000005)

bool saveShellcode();
bool testShellcode();

int __cdecl main()
{
    if (!saveShellcode())
    {
        return 1;
    }
    if (!testShellcode())
    {
        return 2;
    }
    printf_s("save shellcode successfully");
    return 0;
}

bool saveShellcode()
{
#ifdef _WIN64
    FILE* file = fopen("../dist/PELoader_x64.bin", "wb");
#elif _WIN32
    FILE* file = fopen("../dist/PELoader_x86.bin", "wb");
#endif
    if (file == NULL)
    {
        printf_s("failed to create shellcode output file");
        return false;
    }
    uintptr begin = (uintptr)(&Boot);
    uintptr end   = (uintptr)(&Epilogue);
    uintptr size  = end - begin;
    // skip 0xCC instructions at the tail
    uint num0xCC = 0;
    for (;;)
    {
        end--;
        if (*(byte*)end != 0xCC)
        {
            break;
        }
        num0xCC++;
    }
    size -= num0xCC;
    // write shellcode
    size_t n = fwrite((byte*)begin, (size_t)size, 1, file);
    if (n != 1)
    {
        printf_s("failed to save shellcode");
        return false;
    }
    fclose(file);
    return true;
}

bool testShellcode()
{
    // adjust memory protect to RWX
#ifdef _WIN64
    uint hash = 0xEC01DE9C5D56A25C;
    uint key  = 0xD2826BAB71DB502A;
#elif _WIN32
    uint hash = 0x579D4580;
    uint key  = 0x86F8A823;
#endif
    VirtualProtect_t VirtualProtect = FindAPI(hash, key);
    if (VirtualProtect == NULL)
    {
        printf_s("failed to find VirtualProtect\n");
        return false;
    }
    uintptr begin = (uintptr)(&Boot);
    uintptr end   = (uintptr)(&Epilogue);
    uintptr size  = end - begin;
    DWORD protect = PAGE_EXECUTE_READWRITE;
    DWORD old;
    BOOL ok = VirtualProtect((LPVOID)begin, (SIZE_T)size, protect, &old);
    if (!ok)
    {
        printf_s("failed to call VirtualProtect\n");
        return false;
    }
    // simple shellcode test
    errno errno = Boot();
    if (errno != ERR_ARGUMENT_CHECKSUM)
    {
        printf_s("unexpected errno: 0x%lX\n", errno);
        return false;
    }
    return true;
}
