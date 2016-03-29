#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>

const char END = 0xD5; //start/end of file
const char ESC = 0xA5; //escape

int main(int argc, char** argv)
{
	int l = 0;
	if(argc < 3) return 1;
	printf("\nkernupdc: Opening files\n");
	FILE* f = fopen(argv[1], "w");
	if(!f)
	{
		printf("Cannot open serial\n");
		return 1;
	}
	FILE* k = fopen(argv[2], "r");
	if(!k)
	{
		printf("CAnnot open kernel\n");
		fclose(f);
		return 1;
	}
	struct stat buf;
	fstat(fileno(k), &buf);
	int total = buf.st_size;
	printf("\nkernupdc: Writing start\n");
	fwrite(&END, 1, 1, f);
	printf("\nkernupdc: enter loop\n");
	int t = 0;
	while(!feof(k))
	{
		uint8_t c;
		fread(&c, 1, 1, k);
        if(c == END || c == ESC) //escape
            fwrite(&ESC, 1, 1, f);
        fwrite(&c, 1, 1, f);
        t++;
    	int pc = (int)((float)t / (float)total * 100.0f);
    	if(pc % 10 == 0 && pc != l)
    	{
    		printf("Progress: %d/100%%\n", pc);
    		l = pc;
    	}
    }
    printf("\nkernupdc: write end\n");
    for(int i = 0; i < 100; i++) //just to be sure
   		fwrite(&END, 1, 1, f);
    printf("\nkernupdc: close files\n");
    fclose(k);
    fclose(f);
    printf("Press Ctrl+Alt+Delete on the computer running easiOS\n");
    return 0;
}