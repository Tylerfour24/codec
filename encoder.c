#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void makeheaders(FILE *output, const char *empty);

int main(void)
{
	
	const char *textfile = "hello.txt";
	FILE *input = fopen(textfile, "r");
	const char *pcapfile = "hello.pcap";
	FILE *output = fopen(pcapfile, "w+");

	const char empty[1] = {'\0'};

	unsigned int version = 0;
	unsigned int sequence = 0;
	unsigned int type = 0;
	unsigned int source = 0;
	unsigned int dest = 0;
	unsigned int length = 0;
	unsigned char message[20] = {0};
	unsigned int count = 0;

//possible use of strncpy?

//gonna start by writing 82 bytes into the file, all of which should be zero

	makeheaders(output, empty);

	//deal with meditrik header first, then payload

	fscanf(input, "Version: %x", &version);
	fscanf(input, "\nSequence: %x", &sequence);
	fscanf(input, "\nType: %x", &type);
	fscanf(input, "\nSource: %x", &source);
	fscanf(input, "\nDestination: %x", &dest);

	if(type == 3) {
		fscanf(input, "\nLength: %x", &length); //determine length of hello.txt
		length -= 12;
		fscanf(input, "\n");
		for(count = 0; count < length; count++) {
			fscanf(input, "%c", message);
			//printf("%c", message[count]); //never do this again OoO
		}
	}

	//unsigned short wrongway = ( (version << 12) ^ (sequence << 7) ^ type);

	//unsigned short rightway = htons(wrongway); //at the end, when I get all of this into a buffer, do this
	fwrite(message, 12, 1, output);

}

void makeheaders(FILE *output, const char *empty) {
	int length;

	for(length = 0; length < 82; length++) {
		fwrite(empty, 1, 1, output);
	}
}
