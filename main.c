#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08

struct initramfs_file {
  char name[128];
  uint16_t flags;
  uint32_t size;
} __attribute__((packed));

struct initramfs_header {
  uint32_t magic; //0x45524653
  uint32_t count;
  struct initramfs_file files[0];
} __attribute__((packed));

int main(int argc, char** argv) // ./mkinitramfs indir outfile
{
  if(argc < 3)
  {
    printf("Usage: %s indir outfile\n", argv[0]);
    return 1;
  }
  char* dirname = argv[1];
  char* outname = argv[2];
  //initramfs fejléc
  struct initramfs_header header;
  header.magic = 0x45524653;
  header.count = 0;

  FILE* outf; //output file
  DIR *dp; //directory
  struct dirent *ep; //directory entry

  dp = opendir(dirname); //opens the directory

  if(!dp)
  {
    perror("Can't open directory");
    return 1;
  }

  //count files

  while(ep = readdir(dp))
  {
    if(ep->d_name[0] == '.') //skip ., .. and dotfiles
      continue;
    header.count++;
  }

  printf("Count: %d\n", header.count);

  outf = fopen(outname, "wb");
  fwrite(&header, sizeof(struct initramfs_header), 1, outf);

  rewinddir(dp);

  while(ep = readdir(dp))
  {
    if(ep->d_name[0] == '.') //skip ., .. and dotfiles
      continue;
    struct initramfs_file file;
    char fullpath[256];
    memset(&fullpath, 0, 256);
    memset(&file, 0, sizeof(struct initramfs_file));
    //set path
    strcat(fullpath, dirname);
    strcat(fullpath, "/");
    strcat(fullpath, ep->d_name);
    //get size
    struct stat finfo;
    stat(fullpath, &finfo);
    file.size = finfo.st_size;
    //copy filename
    strncpy((char*)&file.name, ep->d_name, 128);
    //write file header
    fwrite(&file, sizeof(struct initramfs_file), 1, outf);
    //write the file itself
    FILE* inf = fopen(fullpath, "rb");
    char buffer[file.size];
    fread(&buffer, file.size, 1, inf);
    fclose(inf);
    fwrite(&buffer, file.size, 1, outf);
    printf("%s\n", fullpath);
    printf("\tSize: %d\n", file.size);
  }
  fclose(outf);
  closedir(dp);

  return 0;
}
