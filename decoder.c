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
	return 0;
}

void readbytes(void) {
	const char *directory = "/usr/local/share/codec/gps.pcap"; //read from this
	char temp[100] = {'0'}; //store in this
	//const char *directory2 = strcat(getenv("HOME"), "work/.git/codec/sample2.txt"); //write to this

	unsigned int count;

	FILE *input = fopen(directory, "r");
	//FILE *output = fopen(directory2, "w");

	//read(descriptor, input, size);
	//write(descriptor2, output, size);

	fseek(input, 0, SEEK_SET); 
	fread(temp, strlen(temp) +1, 1, input);

	for(count = 0; count < strlen(temp); count++) {
		printf("%x\n", temp[count]);
	}

	//successfully reading bytes and printing them to screen. 
}
