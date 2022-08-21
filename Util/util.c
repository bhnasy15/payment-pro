#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "util.h"

bool getInput(char *fmt, char* dest){
	bool isValid = scanf(fmt, dest) == 1;
	while(getchar() != '\n');
	return isValid;
}

bool getFloat(float* dest){
	bool isValid = scanf("%f", dest) == 1;
	while(getchar() != '\n');
	return isValid;
}

bool append(char *path, char *fmt, ...){
	bool isSafe = true;
	FILE *file = fopen(path, "a+");
	if(!file){
		perror(path);
		isSafe = false;
	}

	fprintf(file,"\n");

	va_list args;
	va_start(args, fmt);
		vfprintf(file, fmt, args);
	va_end(args);

	if(fclose(file)){
		perror(path);
		isSafe = false;
	}
	return isSafe;
}

