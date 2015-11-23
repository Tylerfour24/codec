#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
//#include <string.h> //Apparently I no longer need any of these
//#include <unistd.h> //Since the program compiles without them
//#include <sys/types.h> //But I'm leaving them here in case I need them
//#include <sys/stat.h> //In the future.
//#include <fcntl.h>


int getfiletype(FILE *input, int *result);
void readfile(FILE *input, int *result);

int main(void)
{
	const char *directory = "/usr/local/share/codec/hello.pcap"; //this will become argv[1] and subsequent argv[n]s may have to be added
	FILE *input = fopen(directory, "r");
	int *result = malloc(sizeof(int*));

	getfiletype(input, result);
	readfile(input, result);

	/*struct Headers {
		char version;
		char sequence;
		char type;
		char length; this is all struct practice
		char source;
		char dest;
	} header;*/

	free(result);
	return 0;
}

//This function determines the type of packet
int getfiletype(FILE *input, int *result) {

	int mask;
	int value;

	unsigned char temp[200] = {0};
	fseek(input, 83, SEEK_SET); //positioned to read the type (last 3 bits of 1 byte)
	fread(temp, 1, 1, input);

	mask = 0x07;
	value = temp[0];
	*result = mask & value;

	return *result;
}

//this function determines packet length and prints the payload to screen (someday it'll even print it in a readable format)
void readfile(FILE *input, int *result) {

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


	if(*result == 3) {
		printf("MESSAGE\n");
		for(count = 0; count < tempint; count++) {
			printf("%c", next[count]);
		}
	}

	else if (*result == 2) {
		printf("GPS\n");
		for(count = 0; count < tempint; count++) {
			printf("%x", next[count]); //prints hex values until I figure out how to make it human readable
		}
	}

	else if (*result == 1) {
		printf("COMMAND\n");
		for(count = 0; count < tempint; count++) {
			printf("%x", next[count]); //prints hex values until I figure out how to make it human readable
		}
	}

	else if (*result == 0) {
		printf("STATUS\n");
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

/*note byte format of .pcap files is as follows
 *global = 24 bytes
 *packet = 16 bytes
 *ethernet = 14 bytes
 *ipv4 = 20 bytes
 *udp = 8 bytes
 *meditrik header = 12 bytes
 *PAYLOAD = variable, but denoted by value of (length field - 12)
 *ergo, read length field, subract 12 from value, position at byte 94, read newvalue bytes.*/


