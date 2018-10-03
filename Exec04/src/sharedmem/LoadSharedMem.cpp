/*
 * LoadSharedMem.cpp
 *
 *  Created on: 2018. 10. 3.
 *      Author: finegs
 */
// The DLL code

#include <windows.h>
#include <memory.h>

#define SHMEMSIZE 4096

static LPVOID lpvMem = NULL;      // pointer to shared memory
static HANDLE hMapObject = NULL;  // handle to file mapping

// The DLL entry-point function sets up shared memory using a
// named file-mapping object.

BOOL WINAPI DllMain(HINSTANCE hinstDLL,  // DLL module handle
    DWORD fdwReason,              // reason called
    LPVOID lpvReserved)           // reserved
{
    BOOL fInit, fIgnore;

    switch (fdwReason)
    {
        // DLL load due to process initialization or LoadLibrary

          case DLL_PROCESS_ATTACH:

            // Create a named file mapping object

            hMapObject = CreateFileMapping(
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security attributes
                PAGE_READWRITE,         // read/write access
                0,                      // size: high 32-bits
                SHMEMSIZE,              // size: low 32-bits
                TEXT("dllmemfilemap")); // name of map object
            if (hMapObject == NULL)
                return FALSE;

            // The first process to attach initializes memory

            fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

            // Get a pointer to the file-mapped shared memory

            lpvMem = MapViewOfFile(
                hMapObject,     // object to map view of
                FILE_MAP_WRITE, // read/write access
                0,              // high offset:  map from
                0,              // low offset:   beginning
                0);             // default: map entire file
            if (lpvMem == NULL)
                return FALSE;

            // Initialize memory if this is the first process

            if (fInit)
                memset(lpvMem, '\0', SHMEMSIZE);

            break;

        // The attached process creates a new thread

        case DLL_THREAD_ATTACH:
            break;

        // The thread of the attached process terminates

        case DLL_THREAD_DETACH:
            break;

        // DLL unload due to process termination or FreeLibrary

        case DLL_PROCESS_DETACH:

            // Unmap shared memory from the process's address space

            fIgnore = UnmapViewOfFile(lpvMem);

            // Close the process's handle to the file-mapping object

            fIgnore = CloseHandle(hMapObject);

            break;

        default:
          break;
     }

    return TRUE;
    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(lpvReserved);
}

// The export mechanism used here is the __declspec(export)
// method supported by Microsoft Visual Studio, but any
// other export method supported by your development
// environment may be substituted.

#ifdef __cplusplus    // If used by C++ code,
extern "C" {          // we need to export the C interface
#endif

// SetSharedMem sets the contents of the shared memory

__declspec(dllexport) VOID __cdecl SetSharedMem(LPWSTR lpszBuf)
{
    LPWSTR lpszTmp;
    DWORD dwCount=1;

    // Get the address of the shared memory block

    lpszTmp = (LPWSTR) lpvMem;

    // Copy the null-terminated string into shared memory

    while (*lpszBuf && dwCount<SHMEMSIZE)
    {
        *lpszTmp++ = *lpszBuf++;
        dwCount++;
    }
    *lpszTmp = '\0';
}

// GetSharedMem gets the contents of the shared memory

__declspec(dllexport) VOID __cdecl GetSharedMem(LPWSTR lpszBuf, DWORD cchSize)
{
    LPWSTR lpszTmp;

    // Get the address of the shared memory block

    lpszTmp = (LPWSTR) lpvMem;

    // Copy from shared memory into the caller's buffer

    while (*lpszTmp && --cchSize)
        *lpszBuf++ = *lpszTmp++;
    *lpszBuf = '\0';
}
#ifdef __cplusplus
}
#endif



