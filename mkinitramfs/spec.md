EasiOS File System
Version 1

Sector 1: Superblock
	Byte 1-4: magic: Magic number 0x41434754 "ACGT"
	Byte 5-8: nfent: Number of (used AND unused) file entries

Sector 2-2002: Boot sector

Sector 2003-3000
	Data block allocation table
	Byte n: 0x00 - free
			0x01 - used
	

Sector 3000-(3000 + nfent): Entry Blocks
	Byte 1-32: fname: File entry name
	Byte (33 + n4)-(36 + n4): fdbi: File data block indeces

After Entry Blocks: Data blocks (125 sector) (max 4084)
	ddddddddddddaaaaaaaaaaaattttttttttttaaaaaaaaaaaaa

