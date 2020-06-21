#include "patches.h"

#define PATCH(patchname, _address, ...)                \
	static byte patch_sig_##patchname[] = __VA_ARGS__; \
	static const patch patchname = {#patchname, _address, sizeof patch_sig_##patchname / sizeof patch_sig_##patchname[0], patch_sig_##patchname}

#ifdef _WIN32
PATCH(deleted_message_space, 0x3DAE39, {0x20});
PATCH(deleted_message_space_length, 0x174B9E, {0x08});
PATCH(deleted_message_append, 0x174BCC, {0xEB, 0x4C});
PATCH(deleted_message_details, 0xE7C5C, {0xEB});
PATCH(cooldown, 0x14E3B4, {0x90, 0x90, 0xB1, 0x00, 0x90});

const patch* patches[] = {
	&deleted_message_space_length, &deleted_message_details,
	&deleted_message_append, &deleted_message_space, &cooldown};
#elif __linux__
PATCH(deleted_message_details, 0x15EE50,
	  {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});
PATCH(deleted_message_append, 0x204DC9, {0xE9, 0x83, 0x00, 0x00});
PATCH(cooldown, 0x164D70, {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});

const patch* patches[] = {&deleted_message_details, &deleted_message_append, &cooldown};
#elif __APPLE__
PATCH(deleted_message_details, 0x15EE50,
	  {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});
PATCH(deleted_message_append, 0x204DC9, {0xE9, 0x83, 0x00, 0x00});
PATCH(cooldown, 0x11A210, {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});

const patch* patches[] = {&deleted_message_details, &deleted_message_append, &cooldown};
#endif

const byte num_patches = sizeof patches / sizeof patches[0];