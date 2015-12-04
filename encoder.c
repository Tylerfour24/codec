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
	unsigned int paylen = 0;
	unsigned int source = 0;
	unsigned int dest = 0;

	//unsigned char message[20] = {0};
	//unsigned int count = 0;

	makeheaders(output, empty);

	//deal with meditrik header first, then payload

	fscanf(input, "Version: %u", &version);
	fscanf(input, "\nSequence: %u", &sequence);
	fscanf(input, "\nType: %u", &type);
	fscanf(input, "\nSource: %u", &source);
	fscanf(input, "\nDestination: %u", &dest);

	if(type == 3) {
 		fseek(input, 0L, SEEK_END);    
		paylen = ftell(input);
		fseek(input, 0L, SEEK_SET); 
	}

	//unsigned short wrongway = ( (version << 12) ^ (sequence << 7) ^ type);
	version = version << 28;
	printf("version: %d\n", version);
	sequence = sequence << 19;
	printf("sequence: %d\n", sequence);
	type = type << 16;
	printf("type: %d\n", type);

	int wrongway1 = version ^ sequence ^ type ^ paylen;
	printf("wrongway1: %x\n", wrongway1);
	int wrongway2 = source; //possibly revert these to all on one line, but try this way first
	int wrongway3 = dest;
	int rightway[3];
	rightway[1] = htonl(wrongway1);
	rightway[2] = htonl(wrongway2);
	rightway[3] = htonl(wrongway3);
	printf("%x", rightway[1]);
	printf("%x", rightway[2]);
	printf("%x", rightway[3]);

	//unsigned short rightway = htons(wrongway); //at the end, when I get all of this into a buffer, do this
	fwrite(rightway, 12, 1, output);
   // Set seek back to the beginning

}

void makeheaders(FILE *output, const char *empty) {
	int length;

	for(length = 0; length < 82; length++) {
		fwrite(empty, 1, 1, output);
	}
}
