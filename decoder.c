#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readmessage(FILE *input);
int gettype(FILE *input, int *result);

int main(void)
{
	const char *directory = "/usr/local/share/codec/hello.pcap"; //this will become argv[1] and subsequent argv[n]s may have to be added
	FILE *input = fopen(directory, "r");
	int *result = malloc(sizeof(int*)); //holy shit I did this right
	

	gettype(input, result);
	if (*result == 3) { // this is where I need to differentiate between what type of payload I've found
		readmessage(input);
	}
	else if (*result == 2) {
		printf("The result is 2\n");
	}
	else if (*result == 1) {
		printf("The result is 1\n");
	}
	else if (*result == 0) {
		printf("The result is 0\n");
	}
	else {//this does successfully differentiate between the different "type" values
		exit(0);
	}
	//gettype();
	/*struct Headers {
		char version[;
		char sequence;
		char type;
		char length; this is all header practice
		char source;
		char dest;
	} header;*/
	free(result);
	return 0;
}

int gettype(FILE *input, int *result) {

	int mask;
	int value;

	unsigned char temp[200] = {0};
	fseek(input, 83, SEEK_SET); //positioned to read the type (last 3 bits of 1 byte)
	fread(temp, 1, 1, input);

	mask = 0x07;
	value = temp[0];
	*result = mask & value;
	printf("%d\n", *result);

	return *result;
}

void readmessage(FILE *input) {//need to extract length field to know how much to read
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
	for(count = 0; count < tempint; count++) {
		printf("%c", next[count]);
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


