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

void readbytes(void) {
	const char *directory = "/usr/local/share/codec/hello.pcap"; //read from this
	unsigned char temp[200] = {0}; //store in this
	unsigned int count;
	//const char *directory2 = strcat(getenv("HOME"), "work/.git/codec/sample2.txt"); //write to this

	FILE *input = fopen(directory, "r");
	//FILE *output = fopen(directory2, "w");

	//read(descriptor, input, size);
	//write(descriptor2, output, size);

	fseek(input, 0, SEEK_SET); 
	fread(temp, sizeof(temp), 1, input);

	for(count = 0; count < sizeof(temp); count++) {
		printf("%x\n", temp[count]);
	}

	//successfully reading bytes and printing them to screen. 
}
