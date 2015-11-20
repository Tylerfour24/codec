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
	/*struct Headers {
		char version[;
		char sequence;
		char type;
		char length; this is all header practice
		char source;
		char dest;
	} header;*/

	/*int mask1 = 0xf0;
	int mask2 = 0x0f;
	int value = 0xff;
	int result1 = mask1 & value;
	int result2 = mask2 & value; this is all bit masking practice
	printf("%x\n", result1);
	printf("%x\n", result2);
	return 0;*/
}

void readbytes(void) {//need to extract length field to know how much to read
	const char *directory = "/usr/local/share/codec/hello.pcap"; //read from this
	unsigned char temp[200] = {0}; //store in this first
	unsigned char *next; //store in this second
	int tempint;
	int count;

	FILE *input = fopen(directory, "r");

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
} 

/*note byte format of .pcap files is as follows
 *global = 24 bytes
 *packet = 16 bytes
 *ethernet = 14 bytes
 *ipv4 = 20 bytes
 *udp = 8 bytes
 *meditrik header = 12 bytes
 *PAYLOAD = variable, but denoted by value of length field - 12
 *ergo, read length field, subract 12 from value, position at byte 94, read newvalue bytes.*/


