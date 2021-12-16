#if BUILD_WIN32
#define PATCHES_INCLUDE_PATH "win32/patches.inc"
#elif BUILD_LINUX
#define PATCHES_INCLUDE_PATH "linux/patches.inc"
#endif

#define PATCH(patch_name, patch_offset, ...)              \
    global const u8 patch_name##_pattern[] = __VA_ARGS__; \
                                                          \
    global const Patch patch_name = {                     \
        .name    = S8LitComp(#patch_name),                \
        .offset  = patch_offset,                          \
        .pattern = PatternComp(patch_name##_pattern),     \
    };
#include PATCHES_INCLUDE_PATH
#undef PATCH

#define PATCH(patchname, ...) &patchname,
global const Patch* patches[] = {
#include PATCHES_INCLUDE_PATH
};
#undef PATCH

global const u32 patches_count = ArraySize(patches);

#undef PATCHES_INCLUDE_PATH
