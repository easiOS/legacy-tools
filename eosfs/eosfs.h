#ifndef H_EOSFS
#define H_EOSFS

#include <stdint.h>

struct eosfs_header {
	uint8_t magic[8]; //'\033' '\033' '\033' 'E' 'O' 'S' 'F' 'S' 
	uint8_t reserved[504];
} __attribute__((packed));

struct eosfs_file {
	uint8_t filename[8];
	uint8_t extension[3];
	uint8_t flags; //flags
	uint8_t reserved[100];
	uint32_t blocks[100]; //LBA addresses to 64KiB blocks, relative to eofs_header.
} __attribute__((packed));

#endif