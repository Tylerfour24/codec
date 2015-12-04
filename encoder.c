#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int choosefile(int *choice);
void makeheaders(FILE *output, const char *empty);
int craftmeditrik(int *choice);

int main(void)
{
	int *choice = malloc(sizeof(int));
	choice = 0;
	choosefile(choice);
	craftmeditrik(choice);
}

int choosefile(int *choice) {
	printf("Which file are you encoding?\n\n\n0)	Status Pcap\n\n1)	Command Pcap\n\n2)	GPS Pcap\n\n3)	Message Pcap\n\nYour choice here: ");
	scanf("%d", choice);

	return *choice;
}

void makeheaders(FILE *output, const char *empty) {
	int length;

	for(length = 0; length < 82; length++) {
		fwrite(empty, 1, 1, output);
	}
}

int craftmeditrik(int *choice) {

 	const char *textfile;
 	FILE *input;
 	const char *pcapfile;
 	FILE *output;

	switch(*choice) {
		case 0:	
 			textfile = "status.txt";
 			input = fopen(textfile, "r");
 			pcapfile = "status.pcap";
 			output = fopen(pcapfile, "w+");
			break;
		case 1:	
 			textfile = "command.txt";
 			input = fopen(textfile, "r");
 			pcapfile = "command.pcap";
 			output = fopen(pcapfile, "w+");
			break;
		case 2:	
 			textfile = "gps.txt";
 			input = fopen(textfile, "r");
 			pcapfile = "gps.pcap";
 			output = fopen(pcapfile, "w+");
			break;
		case 3:	
 			textfile = "message.txt";
 			input = fopen(textfile, "r");
 			pcapfile = "message.pcap";
 			output = fopen(pcapfile, "w+");
			break;
		default:
			printf("I don't know what you did, but we're done here.");
			exit(0);
	}

	const char empty[1] = {'\0'};

	unsigned int version = 0;
	unsigned int sequence = 0;
	unsigned int type = 0;
	unsigned int paylen = 0;
	unsigned int source = 0;
	unsigned int dest = 0;

	makeheaders(output, empty);

	fscanf(input, "Version: %u", &version);
	fscanf(input, "\nSequence: %u", &sequence); // scan value of 19
	fscanf(input, "\nType: %u", &type);
	fscanf(input, "\nSource: %u", &source);
	fscanf(input, "\nDestination: %u", &dest);

 	fseek(input, 0L, SEEK_END);    
	paylen = ftell(input) - 46;
	fseek(input, 0L, SEEK_SET);

	version <<= 28;
	sequence <<= 21;
	type <<= 16;

	int wrongway1 = version ^ sequence ^ type ^ paylen;
	int wrongway2 = source;
	int wrongway3 = dest;
	int header[3];
	header[0] = wrongway1;
	header[1] = wrongway2;
	header[2] = wrongway3;

	unsigned int rightway = htonl(*header);
	fwrite(&rightway, 12, 1, output);

	return 0;
}
