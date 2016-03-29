#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <netinet/udp.h>

#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

int recv_packet(uint8_t* p, size_t len, FILE* f);

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("pls give serial\n");
		return -1;
	}
	FILE* f = fopen(argv[1], "r");
	if(!f)
	{
		printf("failed to open file\n");
		return -1;
	}
	while(1)
	{
		uint8_t buffer[65536];
		memset(buffer, 0, 65536);
		recv_packet(buffer, 65536, f);
		printf("received packet!\n");
		struct udphdr* udphdr = (struct udphdr*)buffer;
	}
	fclose(f);
	return 0;
}

int recv_packet(uint8_t* p, size_t len, FILE* f)
{
	unsigned char c;
	int received = 0;

	if(!f) return 0;

	while(1)
	{
		c = fread(&c, 1, 1, f);
		switch(c)
		{
			case END:
				if(received)
					return received;
				else
					break;
			case ESC:
				c = fread(&c, 1, 1, f);
				switch(c)
				{
					case ESC_END:
						c = END;
						break;
					case ESC_ESC:
						c = ESC;
						break;
				}
				break;
			default:
				if(received < len)
				{
					p[received++] = c;
				}
				break;
		}
	}
	return 0;
}