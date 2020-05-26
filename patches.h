#ifndef PATCHES_H
#define PATCHES_H

typedef unsigned char byte;

typedef struct {
	const char* name;
	const unsigned address;
	const unsigned sigsize;
	const byte* sig;
} patch;

#define PATCH(patchname, _address, ...)                \
	static byte patch_sig_##patchname[] = __VA_ARGS__; \
	static const patch patchname = {#patchname, _address, sizeof patch_sig_##patchname / sizeof(patch_sig_##patchname[0]), patch_sig_##patchname}

#ifdef _WIN32
/* Windows patches */
PATCH(deleted_message_space_length, 0x1709CE, {0x08});
PATCH(deleted_message_details, 0xE37EC, {0xEB});
PATCH(deleted_message_append, 0x1709FC, {0xEB, 0x4C});
PATCH(deleted_message_space, 0x3D64A9, {0x20});
PATCH(cooldown, 0x14A1E4, {0x90, 0x90, 0xB1, 0x00, 0x90});

static const patch* patches[] = {
	&deleted_message_space_length, &deleted_message_details,
	&deleted_message_append, &deleted_message_space, &cooldown};
#elif defined(__linux__)
/* Linux patches */
PATCH(deleted_message_details, 0x0015EE50,
	  {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});
PATCH(deleted_message_append, 0x00204DC9, {0xE9, 0x83, 0x00, 0x00});

static const patch* patches[] = {&deleted_message_details, &deleted_message_append};
#endif

static const byte num_patches = sizeof patches / sizeof(patches[0]);

#endif
