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

	fscanf(input, "Version: %x", &version);
	fscanf(input, "\nSequence: %x", &sequence);
	fscanf(input, "\nType: %x", &type);
	fscanf(input, "\nSource: %x", &source);
	fscanf(input, "\nDestination: %x", &dest);

	if(type == 3) {
		fscanf(input, "\nLength: %x", &paylen); //determine length of hello.txt
		paylen -= 12;
		/*fscanf(input, "\n");
		for(count = 0; count < paylen; count++) {
			fscanf(input, "%c", message);
			//printf("%c", message[count]); //never do this again OoO
		}*/
	}

	//unsigned short wrongway = ( (version << 12) ^ (sequence << 7) ^ type);
	version = version << 28;
	sequence = sequence << 23;
	type = type << 16;
	
	//the above doesn't work, trying to shift too far. split it up maybe?
	int wrongway1 = version + sequence + type + paylen;
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

}

void makeheaders(FILE *output, const char *empty) {
	int length;

	for(length = 0; length < 82; length++) {
		fwrite(empty, 1, 1, output);
	}
}
