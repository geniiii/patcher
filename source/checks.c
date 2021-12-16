typedef Patch Check;

#if BUILD_WIN32
#define CHECKS_INCLUDE_PATH "win32/checks.inc"
#elif BUILD_LINUX
#define CHECKS_INCLUDE_PATH "linux/checks.inc"
#endif

#define CHECK(check_name, check_offset, ...)              \
    global const Check check_name = {                     \
        .name    = S8LitComp(#check_name),                \
        .offset  = check_offset,                          \
        .pattern = PatternFromStringLitComp(__VA_ARGS__), \
    };
#include CHECKS_INCLUDE_PATH
#undef CHECK

#define CHECK(checkname, ...) &checkname,
global const Check* checks[] = {
#include CHECKS_INCLUDE_PATH
};
#undef CHECK

global const u32 checks_count = ArraySize(checks);

#undef CHECKS_INCLUDE_PATH
