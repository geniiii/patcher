#define BUILD_WIN32 1

#define _AMD64_
#define INVALID_HANDLE_VALUE    ((HANDLE)(LONG_PTR)-1)
#include <windef.h>
#include <errhandlingapi.h>
#include <memoryapi.h>
#include <fileapi.h>

#include "win32.c"
#include "../build.c"
