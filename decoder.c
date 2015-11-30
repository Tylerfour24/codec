#include <stdio.h>
#include <stdlib.h>

int getfiletype(FILE *input, int *type);//create function to get source and destination IDS, or add said functionality into getfiletype function
void readfile(FILE *input, int *type);

int main(void)
{
	const char *directory = "/usr/local/share/codec/hello.pcap"; //this will become argv[1] and subsequent argv[n]s may have to be added
	FILE *input = fopen(directory, "r");
	int *type = malloc(sizeof(int*));

	getfiletype(input, type);
	readfile(input, type);

	free(type);
	return 0;
}

//This function determines the type of packet
int getfiletype(FILE *input, int *type) {

	int mask;
	int value;
	int shiftbits;
	
	unsigned char temp[200] = {0};
	fseek(input, 82, SEEK_SET); //positioned to read version, sequence, and type
	fread(temp, 1, 2, input);
															
	mask = 0x07;
	value = temp[1];
	*type = mask & value;

	shiftbits = temp[0] >> 4;
	printf("Version: %d\n", shiftbits); //prints version #

	mask = 0x0f;
	value = temp[0];
	shiftbits = (mask & temp[0]) + (temp[1] >> 3);
	printf("Sequence: %d\n", shiftbits); //prints sequence #

	fseek(input, 86, SEEK_SET); //positioned to read source and destination IDs
	fread(temp, 1, 8, input);

	printf("Source: %d.%d.%d.%d\n", temp[0], temp[1], temp[2], temp[3]);
	printf("Destination: %d.%d.%d.%d\n", temp[4], temp[5], temp[6], temp[7]);

	return *type;
}

//this function determines packet length and prints the payload to screen (someday it'll even print it in a readable format)
void readfile(FILE *input, int *type) {

	unsigned char temp[200] = {0}; //store in this first
	unsigned char *next; //store in this second
	int tempint;
	int count;

	fseek(input, 84, SEEK_SET); //positioned to read the length (2 bytes)
	fread(temp, 2, 1, input);

	tempint = temp[1];
	next = calloc(tempint, 1);

	fseek(input, 94, SEEK_SET);
	fread(next, 1, tempint, input);


	if(*type == 3) {
		printf("MESSAGE: ");
		for(count = 0; count < tempint; count++) {
			printf("%c", next[count]);
		}
	}

	else if (*type == 2) {
		printf("GPS:\n");
		for(count = 0; count < tempint; count++) {
			printf("%x", next[count]); //prints hex values until I figure out how to make it human readable
		}
	}

	else if (*type == 1) {
		printf("COMMAND:\n");
		for(count = 0; count < tempint; count++) {
			printf("%x", next[count]); //prints hex values until I figure out how to make it human readable
		}
	}

	else if (*type == 0) {
		printf("STATUS:\n");
		for(count = 0; count < tempint; count++) {
			printf("%x", next[count]); //prints hex values until I figure out how to make it human readable
		}
	}

	else {//this successfully differentiates between the different "type" values
		exit(0);
	}

	printf("\n");
	free(next);
}

/*note byte format of .pcap files is as follows:
 *global = 24 bytes
 *packet = 16 bytes
 *ethernet = 14 bytes
 *ipv4 = 20 bytes
 *udp = 8 bytes
 *meditrik header = 12 bytes
 *PAYLOAD = variable, but denoted by value of (length field - 12)
 *:ergo, read length field, subract 12 from value, position at byte 94, read newvalue bytes.*/
