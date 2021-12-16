internal String8 S8_FromCString(char* str) {
    return (String8){
        .cstr = str,
        .size = strlen(str),
    };
}

internal b32 S8_Match(String8 a, String8 b) {
    if (a.size != b.size) {
        return 0;
    }
    
    for (u32 i = 0; i < a.size; ++i) {
        if (a.s[i] != b.s[i]) {
            return 0;
        }
    }
    
    return 1;
}
internal b32 S8_MatchCString(String8 a, const char* b) {
    return S8_Match(a, S8_FromCString(b));
}

internal String8 S8_SubstrOnLastOccurenceOf(String8 str, char c) {
    for (u8* i = str.s + str.size - 1; i >= str.s; --i) {
        if (*i == c) {
            return (String8){
                .s  = i,
                .size = i - str.s,
            };
        }
    }
    
    return str;
}
internal String8 S8_SubstrAfterLastOccurrenceOf(String8 str, char c) {
    String8 on_last = S8_SubstrOnLastOccurenceOf(str, c);
    if (on_last.size - 1 == 0 || on_last.s == str.s) {
        return str;
    }
    return (String8){
        .s    = on_last.s + 1,
        .size = on_last.size - 1
    };
}

internal String8 S8_PathToBaseName(String8 path) {
    String8 result = S8_SubstrAfterLastOccurrenceOf(path, '/');
    if (result.s == path.s) {
        result = S8_SubstrAfterLastOccurrenceOf(path, '\\');
    }
    return result;
}
