#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	
	const char *textfile = "hello.txt";
	FILE *input = fopen(textfile, "r");
	const char *pcapfile = "hello.pcap";
	FILE *output = fopen(pcapfile, "w+");
	int length;
	//gonna use strncpy
	//n needs to be the length of the file
	//char *strncpy(char *dest, const char *src, size_t n)

	fseek(input, 0, SEEK_END);
	length = ftell(input);	
	rewind(input);

	strncpy(&output, &input, length);
}
