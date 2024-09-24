#ifndef PE_LOADER_H
#define PE_LOADER_H

#include "c_types.h"
#include "windows_t.h"
#include "hash_api.h"
#include "errno.h"

typedef errno (*Execute_t)();
typedef errno (*Exit_t)(uint exitCode);
typedef errno (*Destroy_t)();

typedef struct {
    // use custom FindAPI from Gleam-RT for hook
    FindAPI_t FindAPI;

    // PE image memory address
    void* Image;

    // for hook GetCommandLineA and GetCommandLineW,
    // if it is NULL, call original GetCommandLine
    void* CommandLineA;
    void* CommandLineW;

    // set standard handles for hook GetStdHandle,
    // if them are NULL, call original GetStdHandle
    HANDLE StdInput;
    HANDLE StdOutput;
    HANDLE StdError;

    // wait main thread exit if it is a exe image
    bool WaitMain;

    // not erase instructions after call functions about Init or Exit
    bool NotEraseInstruction;

    // adjust current memory page protect
    bool NotAdjustProtect;
} PELoader_Cfg;

typedef struct {
    // absolute memory address about PE entry point
    void* EntryPoint;

    // main thread return value or argument about call ExitProcess
    uint ExitCode;

    // create a thread at EntryPoint, it can call multi times
    Execute_t Execute;

    // release all resource, it can call multi times
    Exit_t Exit;

    // destroy all resource about PE loader, it can only call one time
    Destroy_t Destroy;
} PELoader_M;

// InitPELoader is used to initialize PE loader, it will load PE file
// from memory, but it will not run it, caller must use PELoader_M.
// If failed to initialize, use GetLastError to get error code.
PELoader_M* InitPELoader(PELoader_Cfg* cfg);

#endif // PE_LOADER_H
