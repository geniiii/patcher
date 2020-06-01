#ifndef PATCHES_H
#define PATCHES_H

typedef unsigned char byte;

typedef struct {
	const char* name;
	const unsigned address;
	const unsigned sig_size;
	const byte* sig;
} patch;

extern const patch* patches[];
extern const byte num_patches;

#endif
