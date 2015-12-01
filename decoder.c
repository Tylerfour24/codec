#include <stdio.h>
#include <stdlib.h>

int getfiletype(FILE *input, int *type);
void readfile(FILE *input, int *type);

int main(int argc, char **argv)
{
	const char *directory;
	FILE *input;
	int *type = malloc(sizeof(int*));
	int files;

	for(files = 1; files < argc; files++) {
		//if argument is valid
			directory = argv[files];
			input = fopen(directory, "r");

			getfiletype(input, type);
			readfile(input, type);

		//otherwise
			//error occured, exit program
	}

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

	shiftbits = temp[0] >> 4; //this is printing 4, should be printing 1
	printf("Version: %d\n", shiftbits); //prints version #

	mask = 0x0f;
	value = temp[0];
	shiftbits = (mask & temp[0]) + (temp[1] >> 3);
	printf("Sequence: %d\n", shiftbits); //prints sequence #

	fseek(input, 86, SEEK_SET); //positioned to read source and destination IDs
	fread(temp, 4, 2, input);

	printf("Source: %d\n", temp[0]);
	printf("Destination: %d\n", temp[1]);

	return *type;
}

//this function determines packet length and prints the payload to screen
void readfile(FILE *input, int *type) {

	unsigned char temp[200] = {0}; 
	/*union Buffer {
		unsigned char *cnext;
		double *dnext;
		float *fnext;
	};
	this union is meant to account for the different data types I have to deal with, but let's see if I can do it without first.
	union Buffer buffer;*/
		
	unsigned char *next; //turn this into a union
	int tempint;

	//Consider making the following a switch statement
	if(*type == 3) { //This section works
		int count;

		printf("MESSAGE\n");
		fseek(input, 84, SEEK_SET); //positioned to read the length (2 bytes)
		fread(temp, 1, 2, input);

		tempint = temp[1];
		next = calloc(tempint, 1);

		fseek(input, 94, SEEK_SET);
		fread(next, 1, tempint, input);

		for(count = 0; count < tempint; count++) {
			printf("%c", next[count]);
		}
		free(next);
	}

	else if (*type == 2) { //This section works (tentatively, values could be wrong but it produces values)
		float longitude;
		float latitude;
		float altitude;

		printf("GPS\n");
		next = calloc(2, 8);
		fread(next, 8, 2, input);

		longitude = next[0];
		latitude = next[1];
		
		printf("Longitude: %.2f\n", longitude);
		printf("Latitude: %.2f\n", latitude);
		free(next);

		next = calloc(5, 4);
		fread(next, 4, 5, input);

		altitude = next[4];

		printf("Altitude: %.2f", altitude);
		free(next);
	}

	else if (*type == 1) {
		unsigned short command;
		unsigned short parameter;

		printf("COMMAND\n");
		next = calloc(2, 2);
		fread(next, 2, 2, input);

		command = next[0];
		parameter = next[1];

		printf("Command: %d\n", command);
		switch(command) {
			case 0:
				printf("GET_STATUS");
				break;
			case 1:
				printf("SET_GLUCOSE");
				break;
			case 2:
				printf("GET_GPS");
				break;
			case 3:
				printf("SET_CAPSAICIN");
				break;
			case 4:
				printf("GET_STATUS");
				break;
			case 5:
				printf("SET_OMORFINE");
				break;
			case 6:
				printf("GET_STATUS");
				break;
			case 7:
				printf("REPEAT");
				break;
		}
		if( (command % 2) != 0) {
			printf("\nParameter: %d", parameter);
		}
		free(next);
	}

	else if (*type == 0) { //This prints no matter what for some reason
		double battery;
		unsigned short glucose;
		unsigned short capsaicin;
		unsigned short omorfine;

		printf("STATUS\n");
		next = calloc(1, 8);
		fread(next, 8, 1, input);

		battery = next[0];
		printf("Battery Power: %f\n", battery);
		free(next);

		next = calloc(7, 2);
		fread(next, 2, 7, input);
		
		glucose = next[4];
		capsaicin = next[5];
		omorfine = next[6];
		printf("Glucose: %d\n", glucose);
		printf("Capsaicin: %d\n", capsaicin);
		printf("Omorfine: %d", omorfine);
		free(next);
	}

	else {//this successfully differentiates between the different "type" values
		exit(0);
	}

	printf("\n");
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
