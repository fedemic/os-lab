#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 1000


int main() {

	char path[MAX_LENGTH-5];
	char command[MAX_LENGTH] = "find ";

	printf("Enter path: ");
	scanf("%s", path);
	strcat(command, path);

	system(command);

	return 0;
}