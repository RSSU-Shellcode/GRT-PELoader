#ifndef RUNTIME_H
#define RUNTIME_H

#include "c_types.h"
#include "win_types.h"
#include "dll_kernel32.h"
#include "lib_string.h"
#include "hash_api.h"
#include "errno.h"

// about runtime options at the shellcode tail.
#define OPTION_STUB_SIZE  64
#define OPTION_STUB_MAGIC 0xFC

#define OPT_OFFSET_NOT_ERASE_INSTRUCTION    1
#define OPT_OFFSET_NOT_ADJUST_PROTECT       2
#define OPT_OFFSET_NOT_TRACK_CURRENT_THREAD 3

// for generic shellcode development.

// about library tracker
#ifndef MOD_LIBRARY_H
typedef struct {
    int64 NumModules;
} LT_Status;
#endif // MOD_LIBRARY_H

typedef bool (*LibLockModule_t)(HMODULE hModule);
typedef bool (*LibUnlockModule_t)(HMODULE hModule);
typedef bool (*LibGetStatus_t)(LT_Status* status);
typedef bool (*LibFreeAllMu_t)();

// about memory tracker
#ifndef MOD_MEMORY_H
typedef struct {
    int64 NumGlobals;
    int64 NumLocals;
    int64 NumBlocks;
    int64 NumRegions;
    int64 NumPages;
    int64 NumHeaps;
} MT_Status;
#endif // MOD_MEMORY_H

typedef void* (*MemAlloc_t)(uint size);
typedef void* (*MemCalloc_t)(uint num, uint size);
typedef void* (*MemRealloc_t)(void* ptr, uint size);
typedef void  (*MemFree_t)(void* ptr);
typedef uint  (*MemSize_t)(void* ptr);
typedef uint  (*MemCap_t)(void* ptr);
typedef bool  (*MemLockRegion_t)(LPVOID address);
typedef bool  (*MemUnlockRegion_t)(LPVOID address);
typedef bool  (*MemGetStatus_t)(MT_Status* status);
typedef bool  (*MemFreeAllMu_t)();

// about thread tracker
#ifndef MOD_THREAD_H
typedef struct {
    int64 NumThreads;
    int64 NumTLSIndex;
    int64 NumSuspend;
} TT_Status;
#endif // MOD_THREAD_H

typedef HANDLE (*ThdNew_t)(void* address, void* parameter, bool track);
typedef void   (*ThdExit_t)();
typedef bool   (*ThdLockThread_t)(DWORD id);
typedef bool   (*ThdUnlockThread_t)(DWORD id);
typedef bool   (*ThdGetStatus_t)(TT_Status* status);
typedef bool   (*ThdKillAllMu_t)();

// about resource tracker
#ifndef MOD_RESOURCE_H
typedef struct {
    int64 NumMutexs;
    int64 NumEvents;
    int64 NumFiles;
    int64 NumDirs;
} RT_Status;
#endif // MOD_RESOURCE_H

typedef bool (*ResLockMutex_t)(HANDLE hMutex);
typedef bool (*ResUnlockMutex_t)(HANDLE hMutex);
typedef bool (*ResGetStatus_t)(RT_Status* status);
typedef bool (*ResFreeAllMu_t)();

// about argument store
typedef bool (*ArgGetValue_t)(uint32 id, void* value, uint32* size);
typedef bool (*ArgGetPointer_t)(uint32 id, void** pointer, uint32* size);
typedef bool (*ArgErase_t)(uint32 id);
typedef void (*ArgEraseAll_t)();

// about in-memory storage
typedef bool (*ImsSetValue_t)(int id, void* value, uint size);
typedef bool (*ImsGetValue_t)(int id, void* value, uint* size);
typedef bool (*ImsGetPointer_t)(int id, void** pointer, uint* size);
typedef bool (*ImsDelete_t)(int id);
typedef bool (*ImsDeleteAll_t)();

// about WinBase
// The buffer allocated from methods must call Runtime_M.Memory.Free().
typedef UTF16 (*ANSIToUTF16_t)(ANSI s);
typedef ANSI  (*UTF16ToANSI_t)(UTF16 s);
typedef UTF16 (*ANSIToUTF16N_t)(ANSI s, int n);
typedef ANSI  (*UTF16ToANSIN_t)(UTF16 s, int n);

// about WinFile
// The buffer allocated from ReadFile must call Runtime_M.Memory.Free().
typedef errno (*ReadFileA_t)(LPSTR path, byte** buf, uint* size);
typedef errno (*ReadFileW_t)(LPWSTR path, byte** buf, uint* size);
typedef errno (*WriteFileA_t)(LPSTR path, byte* buf, uint size);
typedef errno (*WriteFileW_t)(LPWSTR path, byte* buf, uint size);

// about WinHTTP
#ifndef WIN_HTTP_H
// The HTTP_Body.Buf allocated from WinHTTP must call Runtime_M.Memory.Free().
typedef struct {
    void* Buf;
    uint  Size;
} HTTP_Body;

typedef struct {
    UTF16  Headers;     // split by "\r\n"
    UTF16  ContentType; // for POST method
    UTF16  UserAgent;   // default User-Agent
    UTF16  ProxyURL;    // http://user:pass@host.com/
    uint   MaxBodySize; // default is no limit
    uint32 Timeout;     // millseconds
    uint8  AccessType;  // reference document about WinHttpOpen

    HTTP_Body* Body;
} HTTP_Opts;

typedef struct {
    int32 StatusCode;
    UTF16 Headers;

    HTTP_Body Body;
} HTTP_Resp;
#endif // WIN_HTTP_H

typedef errno (*HTTPGet_t)(UTF16 url, HTTP_Opts* opts, HTTP_Resp* resp);
typedef errno (*HTTPPost_t)(UTF16 url, HTTP_Body* body, HTTP_Opts* opts, HTTP_Resp* resp);
typedef errno (*HTTPDo_t)(UTF16 url, UTF16 method, HTTP_Opts* opts, HTTP_Resp* resp);
typedef errno (*HTTPFree_t)();

// about WinCrypto

// about random module
typedef void   (*RandBuffer_t)(byte* buf, int64 size);
typedef bool   (*RandBool_t)(uint64 seed);
typedef int64  (*RandInt64_t)(uint64 seed);
typedef uint64 (*RandUint64_t)(uint64 seed);
typedef int64  (*RandInt64N_t)(uint64 seed, int64 n);
typedef uint64 (*RandUint64N_t)(uint64 seed, uint64 n);

// about crypto module
typedef void (*Encrypt_t)(byte* buf, uint size, byte* key, byte* iv);
typedef void (*Decrypt_t)(byte* buf, uint size, byte* key, byte* iv);

// about compress module
// 
// In general, the size of the destination buffer is 1.125
// times the size of the source buffer to handle the worst
// case (no compressible data).
// 
// Since the algorithm is relatively simple to implement, 
// it is NOT recommended to compress data exceeding 1MB.
// 
// Please record the original size when compressing to 
// facilitate decompression°£
typedef uint (*Compress_t)(void* dst, void* src, uint len, uint window);
typedef uint (*Decompress_t)(void* dst, void* src, uint len);

// GetProcAddress, GetProcAddressByName and GetProcAddressByHash
// are use Hash API module for implement original GetProcAddress.
// GetProcAddressOriginal is not recommend, usually use
// GetProcAddressByName with hook FALSE instead it.
// These methods are used for IAT hooks or common shellcode.
typedef void* (*GetProcByName_t)(HMODULE hModule, LPCSTR lpProcName, bool hook);
typedef void* (*GetProcByHash_t)(uint hash, uint key, bool hook);
typedef void* (*GetProcOriginal_t)(HMODULE hModule, LPCSTR lpProcName);

// about runtime core methods
//
// It is NOT recommended use "Hide" and "Recover", these functions
// are used to test and research, if use them, runtime will loss
// the shield protect and structure data encrypt.
//
// SleepHR is used to call Hide, Sleep and Recover, usually it called by hook.
// Cleanup is used to clean all tracked object except locked.
// Exit is used to clean all tracked object and clean runtime self.
typedef struct {
    LT_Status Library;
    MT_Status Memory;
    TT_Status Thread;
    RT_Status Resource;
} Runtime_Metrics;

typedef errno (*SleepHR_t)(uint32 milliseconds);
typedef errno (*Hide_t)();
typedef errno (*Recover_t)();
typedef errno (*Metrics_t)(Runtime_Metrics* metrics);
typedef errno (*Cleanup_t)();
typedef errno (*Exit_t)();

// Runtime_M contains exported runtime methods.
typedef struct {
    struct {
        FindAPI_t   FindAPI;
        FindAPI_A_t FindAPI_A;
        FindAPI_W_t FindAPI_W;
    } HashAPI;

    struct {
        LoadLibraryA_t   LoadA;
        LoadLibraryW_t   LoadW;
        LoadLibraryExA_t LoadExA;
        LoadLibraryExW_t LoadExW;
        FreeLibrary_t    Free;
        GetProcAddress_t GetProc;

        LibLockModule_t   Lock;
        LibUnlockModule_t Unlock;
        LibGetStatus_t    Status;
        LibFreeAllMu_t    FreeAll;
    } Library;

    struct {
        MemAlloc_t   Alloc;
        MemCalloc_t  Calloc;
        MemRealloc_t Realloc;
        MemFree_t    Free;
        MemSize_t    Size;
        MemCap_t     Cap;

        MemLockRegion_t   Lock;
        MemUnlockRegion_t Unlock;
        MemGetStatus_t    Status;
        MemFreeAllMu_t    FreeAll;
    } Memory;

    struct {
        ThdNew_t  New;
        ThdExit_t Exit;
        Sleep_t   Sleep;

        ThdLockThread_t   Lock;
        ThdUnlockThread_t Unlock;
        ThdGetStatus_t    Status;
        ThdKillAllMu_t    KillAll;
    } Thread;

    struct {
        ResLockMutex_t   LockMutex;
        ResUnlockMutex_t UnlockMutex;
        ResGetStatus_t   Status;
        ResFreeAllMu_t   FreeAll;
    } Resource;

    struct {
        ArgGetValue_t   GetValue;
        ArgGetPointer_t GetPointer;
        ArgErase_t      Erase;
        ArgEraseAll_t   EraseAll;
    } Argument;

    struct {
        ImsSetValue_t   SetValue;
        ImsGetValue_t   GetValue;
        ImsGetPointer_t GetPointer;
        ImsDelete_t     Delete;
        ImsDeleteAll_t  DeleteAll;
    } Storage;

    struct {
        ANSIToUTF16_t  ANSIToUTF16;
        UTF16ToANSI_t  UTF16ToANSI;
        ANSIToUTF16N_t ANSIToUTF16N;
        UTF16ToANSIN_t UTF16ToANSIN;
    } WinBase;

    struct {
        ReadFileA_t  ReadFileA;
        ReadFileW_t  ReadFileW;
        WriteFileA_t WriteFileA;
        WriteFileW_t WriteFileW;
    } WinFile;
    
    struct {
        HTTPGet_t  Get;
        HTTPPost_t Post;
        HTTPDo_t   Do;
        HTTPFree_t Free;
    } WinHTTP;

    struct {
        RandBuffer_t  Buffer;
        RandBool_t    Bool;
        RandInt64_t   Int64;
        RandUint64_t  Uint64;
        RandInt64N_t  Int64N;
        RandUint64N_t Uint64N;
    } Random;

    struct {
        Encrypt_t Encrypt;
        Decrypt_t Decrypt;
    } Crypto;

    struct {
        Compress_t   Compress;
        Decompress_t Decompress;
    } Compressor;

    struct {
        GetProcByName_t   GetProcByName;
        GetProcByHash_t   GetProcByHash;
        GetProcOriginal_t GetProcOriginal;
    } IAT;

    struct {
        SleepHR_t Sleep;
        Hide_t    Hide;
        Recover_t Recover;
        Metrics_t Metrics;
        Cleanup_t Cleanup;
        Exit_t    Exit;
    } Core;

    struct {
        HANDLE Mutex;
    } Data;

    ExitProcess_t ExitProcess;
} Runtime_M;

typedef struct {
    // protect instructions like shellcode before Runtime,
    // if it is NULL, Runtime will only protect self.
    void* BootInstAddress;

    // not erase runtime instructions after call Runtime_M.Exit
    bool NotEraseInstruction;

    // not adjust current memory page protect for erase runtime.
    bool NotAdjustProtect;

    // track current thread for test or debug mode.
    bool TrackCurrentThread;
} Runtime_Opts;

// InitRuntime is used to initialize runtime and return module methods.
// If failed to initialize, use GetLastError to get error code.
extern Runtime_M* InitRuntime(Runtime_Opts* opts);

// reserve stub for store arguments
#pragma warning(push)
#pragma warning(disable: 4276)
extern void Argument_Stub();
#pragma warning(pop)

#endif // RUNTIME_H
