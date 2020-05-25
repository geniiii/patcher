// TODO: macOS support, Linux patches

#ifdef _WIN32
#define DEFAULT_FILENAME "Ripcord.exe"
#define _CRT_SECURE_NO_WARNINGS 1
#else
#define DEFAULT_FILENAME "Ripcord"
#endif 

#include <stdio.h>
#include <stdlib.h>
#include "patches.h"

int main(int argc, char** argv) {
	FILE* fp = NULL;
	if (argc == 1) {
		fputs(
			"No arguments passed, attempting to use Ripcord.exe as original "
			"binary...\n",
			stderr);
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

	for (unsigned i = 0; i < NUM_PATCHES; ++i) {
		const patch* p = patches[i];
		printf("Patch \"%s\":\n", p->name);
		for (unsigned j = 0; j < p->sigsize; ++j) {
			printf("0x%.2x: 0x%.2x -> 0x%.2x\n", p->address + j,
				   buf[p->address + j], p->sig[j]);
			buf[p->address + j] = p->sig[j];
		}
		putchar('\n');
	}

	fp = fopen(argc <= 1 ? "RipcordPatched.exe" : argv[2], "wb");
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