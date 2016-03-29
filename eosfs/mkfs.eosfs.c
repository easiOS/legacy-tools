#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "eosfs.h"

int format_fs(FILE* f)
{
	if(!f)
		return ENOENT;
	//write header
	struct eosfs_header hdr;
	memcpy(hdr.magic, "\033\033\033EOSFS", 8);
	fwrite(&hdr, sizeof(struct eosfs_header), 1, f);
	//write file entries
	struct eosfs_file file;
	memset(&file, 0, 512);
	fwrite(&file, sizeof(struct eosfs_file), 32768, f);
	return 0;
}

int main(int argc, char** argv)
{
	FILE* f = fopen("test", "wb");
	format_fs(f);
	fclose(f);
	return 0;
}