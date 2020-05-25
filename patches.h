#ifndef PATCHES_H
#define PATCHES_H

typedef unsigned char byte;

typedef struct {
	const char* name;
	const unsigned address;
	const unsigned sigsize;
	const byte* sig;
} patch;

#define PATCH(patchname, size, _address, ...)                   \
	static byte patch_sig_##patchname[size] = __VA_ARGS__;      \
	static const patch patchname = {#patchname, _address, size, \
									patch_sig_##patchname}

PATCH(deleted_message_space_length, 1, 0x1709CE, {0x08});
PATCH(deleted_message_details, 1, 0xE37EC, {0xEB});
PATCH(deleted_message_append, 2, 0x1709FC, {0xEB, 0x4C});
PATCH(deleted_message_space, 1, 0x3D64A9, {0x20});

PATCH(cooldown, 5, 0x14A1E4, {0x90, 0x90, 0xB1, 0x00, 0x90});

#define NUM_PATCHES 5
static const patch* patches[NUM_PATCHES] = {
	&deleted_message_space_length, &deleted_message_details,
	&deleted_message_append, &deleted_message_space, &cooldown};

#endif