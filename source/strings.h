typedef struct {
    union {
        u8*   s;
        u8*   str;
        char* cstr;
    };
    u64 size;
} String8;

// NOTE(geni): What are the Comp macros for? MSVC. They're for MSVC. Try replacing S8LitComp with S8Lit in patches/checks.c and see for yourself.
//             (disclaimer: I don't know if it's complying with the standard or just stupid, but clang handles it fine)
#define S8Lit(str) (String8) S8LitComp(str)
#define S8LitComp(str) \
{ .s = (u8*) str, .size = (i32) ArraySize(str) - 1 }

#define S8VArg(str) (i32)((str).size), ((str).s)
