#define FUSE_USE_VERSION 30

#include <fuse/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "../eosfs.h"

struct eosfs_file_cache {
	char filename[8];
	char extension[3];
	uint8_t flags; //flags
	uint32_t blocks[100];
} fcache[32768];
FILE* fs;

static int eosfs_getattr(const char* path, struct stat* stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0)
	{
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}
	else
	{
		path++;
		for(int i = 0; i < 32768; i++)
		{
			if(strcmp(path, fcache[i].filename) == 0)
			{
				stbuf->st_mode = S_IFREG | 0444;
				stbuf->st_nlink = 1;
				int size = 0;
				for(int j = 0; j < 100; j++)
				{
					if(fcache[i].blocks[j] != 0)
					{
						size += 65536;
					}
				}
				stbuf->st_size = size;
				return 0;
			}
		}
	}
	return -ENOENT;
}

static int eosfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
						off_t offset, struct fuse_file_info* fi)
{
	if(strcmp(path, "/") != 0)
		return -ENOENT;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	char buffer[]
	for(int i = 0; i < 32768; i++)
	{
		if(fcache[i].filename[0] == '\0') continue;
		filler(buf, fcache[i].filename, NULL, 0);
	}
	return 0;
}

struct fuse_operations eosfs_oper = {
  .getattr = eosfs_getattr,
  .readdir = eosfs_readdir
};

int main(int argc, char** argv)
{	
	if(argc < 3)
	{
		printf("Not enough parameters\n");
		return 1;
	}
	fs = fopen(argv[1], "rb+");
	if(!fs)
	{
		printf("%s no such file\n", argv[1]);
		return 1;
	}

	struct eosfs_header hdr;
	fread(&hdr, sizeof(struct eosfs_header), 1, fs);
	if(memcmp(hdr.magic, "\033\033\033EOSFS", 8) != 0)
	{
		printf("Bad magic\n");
		fclose(fs);
		return 0;
	}
	for(int i = 0; i < 32768; i++)
	{
		struct eosfs_file eosfsf;
		fread(&eosfsf, sizeof(struct eosfs_file), 1, fs);
		memcpy(&fcache[i], &eosfsf, 12);
		memcpy(fcache[i].blocks, eosfsf.blocks, 400);
	}

	argc--; argv++;
	return fuse_main(argc, argv, &eosfs_oper, NULL);
}