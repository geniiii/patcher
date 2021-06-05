internal String8 S8FromCString(char* str) {
	return (String8){
		.s	  = (u8*) str,
		.size = strlen(str),
	};
}

internal b32 S8Compare(String8 a, String8 b) {
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
internal b32 S8CompareToCString(String8 a, const char* b) {
	return S8Compare(a, S8FromCString(b));
}
internal b32 CStringCompareToS8(const char* a, String8 b) {
	return S8Compare(S8FromCString(a), b);
}

internal String8 S8SubstringAfterLastOccurenceOf(String8 str, char c) {
	for (u8* i = str.s + str.size; i >= str.s; --i) {
		if (*i == c) {
			return (String8){
				.s	  = i + 1,
				.size = i + 1 - str.s,
			};
		}
	}

	return str;
}
internal String8 S8PathToBaseName(String8 path) {
	String8 result = S8SubstringAfterLastOccurenceOf(path, '/');
	if (result.s == path.s) {
		result = S8SubstringAfterLastOccurenceOf(path, '\\');
	}
	return result;
}
