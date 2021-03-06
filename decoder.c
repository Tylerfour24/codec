#include <stdio.h>
#include <stdlib.h>

int getfiletype(FILE *input, int *type);
void readfile(FILE *input, int *type);
void printmessage(FILE *input, unsigned char *next, int tempint, unsigned char *temp);
void printgps(FILE *input, unsigned char *next);
void printcommand(FILE *input, unsigned char *next);
void printstatus(FILE *input, unsigned char *next);

int main(int argc, char **argv)
{
	const char *directory;
	FILE *input;
	int *type = malloc(sizeof(int*));
	int files;

	for(files = 1; files < argc; files++) {
		//if path denotes pcap file (implement if/else if time permits)
			directory = argv[files];
			input = fopen(directory, "r");

			getfiletype(input, type);
			readfile(input, type);

			printf("\n");

		//otherwise
			//error occured, exit program
	}

	free(type);
	return 0;
}

//function responsible for determining pcap type
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

	printf("Type: %d\n", *type); //prints type #

	fseek(input, 86, SEEK_SET); //positioned to read source and destination IDs
	fread(temp, 4, 2, input);

	printf("Source: %d\n", temp[0]);
	printf("Destination: %d\n", temp[1]);

	return *type;
}

//function that calls proper function to handle payload
void readfile(FILE *input, int *type) {
	unsigned char temp[200] = {0};
	unsigned char *next = {0};
	int tempint = 0;

	if(*type == 3) {
		printmessage(input, next, tempint, temp);
	}

	else if (*type == 2) {
		printgps(input, next);
	}

	else if (*type == 1) {
		printcommand(input, next);
	}

	else if (*type == 0) {
		printstatus(input, next);
	}

	else {
		exit(0);
	}

	printf("\n");
}

//function responsible for message payload
void printmessage(FILE *input, unsigned char *next, int tempint, unsigned char *temp) {
	int count;

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

//function responsible for gps payload
void printgps(FILE *input, unsigned char *next) {
	float longitude;
	float latitude;
	float altitude;

	next = calloc(2, 8);
	fread(next, 8, 2, input);

	longitude = next[0];
	latitude = next[1];
		
	printf("Longitude: %.2f\n", longitude);
	printf("Latitude: %.2f\n", latitude);
	free(next);

	next = calloc(1, 4);
	fread(next, 4, 1, input);

	altitude = next[0];

	printf("Altitude: %.2f", altitude);
	free(next);
}

//function responsible for command payload
void printcommand(FILE *input, unsigned char *next) {
	unsigned short command;
	unsigned short parameter;

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

//function responsible for status payload
void printstatus(FILE *input, unsigned char *next) {
	double battery;
	unsigned short glucose;
	unsigned short capsaicin;
	unsigned short omorfine;

	next = calloc(1, 8);
	fread(next, 8, 1, input);

	battery = next[0];
	printf("Battery Power: %f\n", battery);
	free(next);

	next = calloc(3, 2);
	fread(next, 2, 3, input);
		
	glucose = next[4];
	capsaicin = next[5];
	omorfine = next[6];
	printf("Glucose: %d\n", glucose);
	printf("Capsaicin: %d\n", capsaicin);
	printf("Omorfine: %d", omorfine);
	free(next);
}
