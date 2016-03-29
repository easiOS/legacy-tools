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

struct eofs_super
{
  
}


int main(int argc, char** argv) // ./mkinitramfs indir outfile
{
  if(argc < 3)
  {
    printf("Usage: %s indir outfile\n", argv[0]);
    return 1;
  }

  
  
  return 0;
}
