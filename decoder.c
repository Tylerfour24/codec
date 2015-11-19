#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readbytes(void);

int main(void)
{
	readbytes();
	/*struct Headers {
		char version[;
		char sequence;
		char type;
		char length;
		char source;
		char dest;
	} header;*/

	int mask1 = 0xf0;
	int mask2 = 0x0f;
	int value = 0xff;
	int result1 = mask1 & value;
	int result2 = mask2 & value;
	printf("%x\n", result1);
	printf("%x\n", result2);
	return 0;
}

void readbytes(void) {//need to extract length field to know how much to read
	const char *directory = "/usr/local/share/codec/hello.pcap"; //read from this
	unsigned char temp[200] = {0}; //store in this
	int tempint;
	int count;

	FILE *input = fopen(directory, "r");
	//FILE *output = fopen(directory2, "w");

	//read(descriptor, input, size);
	//write(descriptor2, output, size);

	fseek(input, 84, SEEK_SET); //positioned to read the length (2 bytes)
	fread(temp, 2, 1, input);

	//char *value = strcat(temp[0], temp[1]);
	//printf("%s\n", value);
	tempint = temp[1];
	unsigned char *next = calloc(tempint, 1); //calloc is supposed to be used for allocating heap memory for arrays. But this isn't working.
	fseek(input, 82, SEEK_SET);
	fread(next, 1, tempint, input);
	for(count = 0; count < tempint; count++) {
		printf("%x", next[count]);
	}
	printf("\n");
}
