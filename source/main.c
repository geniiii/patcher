i32 main(i32 argc, char** argv) {
	// TODO(geni): Rewrite this whole thing
	// TODO(geni): Memory-mapped files

	if (argc == 2) {
		if (CStringCompareToS8(argv[1], S8Lit("-h"))) {
			String8 base_name = S8PathToBaseName(S8FromCString(argv[0]));
			printf("Usage: %.*s [-h] [-l] [original (" DEFAULT_FILENAME ") [patched (" DEFAULT_PATCH_FILENAME ") [disabled patches]]]", S8Expand(base_name));
		} else if (CStringCompareToS8(argv[1], S8Lit("-l"))) {
			puts("Available patches:");
			for (u32 i = 0; i < patches_count; ++i) {
				printf("Patch \"%.*s\"\n", S8Expand(patches[i]->name));
			}
		}

		return 0;
	}

	// NOTE(geni): User wants to disable specific patches
	if (argc > 3) {
		for (i32 i = 3; i < argc; ++i) {
			for (u32 p = 0; p < patches_count; ++p) {
				if (!patches[p]) {
					continue;
				}

				if (S8CompareToCString(patches[p]->name, argv[i])) {
					patches[p] = NULL;
				}
			}
		}
	}

	FILE* fp = NULL;
	if (argc == 1) {
		fputs("No arguments passed, attempting to use " DEFAULT_FILENAME " as original binary...\n", stderr);
		fp = fopen(DEFAULT_FILENAME, "rb");
	} else {
		fp = fopen(argv[1], "rb");
	}

	if (!fp) {
		fputs("Failed to open original binary!\n", stderr);
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	const u64 size = ftell(fp);
	rewind(fp);

	u8* buf = malloc(size);
	if (!buf) {
		fprintf(stderr, "Failed to allocate buffer (size %zu), not enough memory?", size);
		fclose(fp);
		return 1;
	}
	fread(buf, size, sizeof(u8), fp);
	fclose(fp);

	b32 failed_checks = 0;
	for (u32 i = 0; i < checks_count; ++i) {
		const Check* c = checks[i];

		if (memcmp(buf + c->offset, PatternExpand(c->pattern)) != 0) {
			fprintf(stderr, "Check \"%.*s\" failed!\n", S8Expand(c->name));
			failed_checks = 1;
		}
	}
	if (failed_checks) {
		return 1;
	}

	for (u32 i = 0; i < patches_count; ++i) {
		const Patch* p = patches[i];

		// NOTE(geni): Patch is (hopefully) disabled
		if (!p) {
			continue;
		}

		memcpy(buf + p->offset, PatternExpand(p->pattern));
		printf("Patch \"%.*s\" done\n", S8Expand(p->name));
	}

	if (argc < 2) {
		fputs("No patched filename passed, attempting to use " DEFAULT_PATCH_FILENAME " as patched binary...\n", stderr);
		fp = fopen(DEFAULT_PATCH_FILENAME, "wb");
	} else {
		fp = fopen(argv[2], "wb");
	}

	if (!fp) {
		fputs("Failed to create patched binary!\n", stderr);
		return 1;
	}
	const u64 written = fwrite(buf, sizeof(buf[0]), size, fp);
	if (written != size) {
		fputs("Number of bytes written differs from original size\n", stderr);
		return 1;
	}

	fclose(fp);
	free(buf);
}
