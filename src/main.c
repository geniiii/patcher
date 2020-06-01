#ifdef _WIN32
#define DEFAULT_FILENAME "Ripcord.exe"
#define DEFAULT_PATCH_FILENAME "RipcordPatched.exe"
#define _CRT_SECURE_NO_WARNINGS 1
#elif __linux__
#define DEFAULT_FILENAME "Ripcord"
#define DEFAULT_PATCH_FILENAME "RipcordPatched"
#endif

#define MATCH(a, b) strcmp(a, b) == 0

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checks.h"
#include "patches.h"

/* I hope no (supported) OS ever uses anything different.. */
const char* portable_ish_basename(const char* path) {
	char* base = strrchr(path, '/');
	if (!base) {
		base = strrchr(path, '\\');
	}
	return base ? base + 1 : path;
}

void usage(const char* path) {
	printf("Usage: %s [-h] [-l] [original (" DEFAULT_FILENAME ") [patched (" DEFAULT_PATCH_FILENAME ") [disabled patches]]]", portable_ish_basename(path));
}

void list() {
	for (unsigned i = 0; i < num_patches; ++i) {
		printf("Patch \"%s\"\n", patches[i]->name);
	}
}

int main(int argc, char** argv) {
	if (argc == 2) {
		if (MATCH(argv[1], "-h")) {
			usage(argv[0]);
		} else if (MATCH(argv[1], "-l")) {
			list();
		}

		return 0;
	}

	/* User wants to disable specific patches */
	if (argc > 3) {
		for (unsigned i = 3; i < argc; ++i) {
			for (unsigned p = 0; p < num_patches; ++p) {
				/* Patch may already be disabled (or NULL for some other reason) */
				if (!patches[p]) {
					continue;
				}

				if (MATCH(patches[p]->name, argv[i])) {
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
	const size_t size = ftell(fp);
	rewind(fp);

	byte* buf = malloc(sizeof(byte) * size);
	if (!buf) {
		fprintf(stderr,
				"Failed to allocate buffer (size %zu), not enough memory?",
				sizeof(byte) * size);
		fclose(fp);
		return 1;
	}
	fread(buf, size, sizeof(byte), fp);
	fclose(fp);

	for (unsigned i = 0; i < num_checks; ++i) {
		const check* c = checks[i];

		for (unsigned j = 0; j < c->sig_size; ++j) {
			if (buf[c->address + j] != c->sig[j]) {
				fprintf(stderr, "Check \"%s\" failed!\n", c->name);

				/* Don't return if there are any more checks left to do, it's better to let all of them run */
				if (i == num_checks - 1) {
					return 1;
				}

				break;
			}
		}
	}

	for (unsigned i = 0; i < num_patches; ++i) {
		const patch* p = patches[i];

		/* Patch is (probably) disabled */
		if (!p) {
			continue;
		}

		printf("Patch \"%s\":\n", p->name);
		for (unsigned j = 0; j < p->sig_size; ++j) {
			printf("0x%.2x: 0x%.2x -> 0x%.2x\n", p->address + j, buf[p->address + j], p->sig[j]);
			buf[p->address + j] = p->sig[j];
		}
		putchar('\n');
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
	const size_t written = fwrite(buf, sizeof(byte), size, fp);
	if (written != size) {
		fputs("Number of bytes written differs from original size", stderr);
		return 1;
	}
	fclose(fp);
	free(buf);
}
