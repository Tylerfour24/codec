#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	printf("One.\n");
	const char *directory = "/usr/local/share/codec/gps.pcap"; //read from this
	const char *directory2 = strcat(getenv("HOME"), "work/.git/codec/sample2"); //write to this
	printf("Two.\n");

	int size = 1; 
	
	int descriptor = open(directory, O_RDONLY);
	int descriptor2 = open(directory2, O_WRONLY);
	printf("Three.\n");

	FILE *input = fopen(directory, "r");
	FILE *output = fopen(directory2, "w");
	printf("Four.\n");

	read(descriptor, input, size);
	write(descriptor2, output, size);
	printf("Five.\n");

	fseek(input, 0, SEEK_SET); 
	printf("Six.\n");
	fread(&output, 1, 1, input); //i had input and output switched. oops.
	printf("Seven.\n");
	//this should read a byte from input and write it to output. it seems to be working. need to check, aka find out where output is.
	return 0;
}
