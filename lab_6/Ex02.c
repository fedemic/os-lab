#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20

char phrase[MAX_SIZE];		//global string shared

void *thread_func() {

	while(1) {
		if(strcmp(phrase, "casa") == 0 || strcmp(phrase, "house") == 0) {	// whenever 'house' or 'casa' is detected report it
			printf("Thread: house detected.\n");
		}
		sleep(1);		// scan global variable every second
	}
}

int main() {

	pthread_t thread_id;

	if(pthread_create(&thread_id, NULL, thread_func, NULL) != 0) {		// thread generation and error checking
		printf("Error creating thread.\n");
		exit(1);
	}

	while(1) {
		printf("Enter a string: ");			// ask for a string to be saved in global variable
		scanf("%s", phrase);

		if(strcmp(phrase, "exit") == 0) {		// program exit condition
			printf("Exiting program...\n");
			break;
		}

		sleep(1);
	}

	return 0;
}