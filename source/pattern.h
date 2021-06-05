typedef struct {
	const u8* pattern;
	u32		  size;
} Pattern;

// NOTE(geni): What are the Comp macros for? MSVC. They're for MSVC. Try replacing PatternComp with Pattern in patches/checks.c and see for yourself.
// 			   (disclaimer: I don't know if it's complying with the standard or just stupid, but clang handles it fine)
#define Pattern(p) (Pattern) PatternComp(p)
#define PatternComp(p) \
	{ .pattern = (const u8*) (p), .size = ArraySize(p) }

#define PatternFromStringLit(p) (Pattern) PatternFromStringLitComp(p)
#define PatternFromStringLitComp(p) \
	{ .pattern = (const u8*) (p), .size = ArraySize(p) - 1 }

#define PatternExpand(p) (p).pattern, (p).size
