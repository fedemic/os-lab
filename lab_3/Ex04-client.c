#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_LENGTH 1000


int main() {

	int shmid;
	key_t key;
	char *str;
	char input[MAX_LENGTH];
	int quit = 0;


	key = ftok("/tmp", 'A');				// generating unique key for shared segment
	shmid = shmget(key, MAX_LENGTH, IPC_CREAT | 0777);		// create shared segment

	if(shmid < 0) {
		printf("Error creating shared segment\n");
		exit(1);
	}

	while(quit == 0) {
		str = (char *)shmat(shmid, NULL, 0);		// attach segment
		if(str == (char *)-1) {
			printf("Error in shmat client\n");
			exit(1);
		}

		printf("Enter a command: ");
		fgets(input, MAX_LENGTH, stdin);	// command is read from terminal
		input[strcspn(input, "\n")] = 0;	// remove newline added by fgets function
		strcpy(str, input);					// command is put into segment
		printf("Executing %s\n", str);

		if(strcmp(str, "exit") == 0) { 	// exit condition
			quit = 1;
		}

		shmdt(str); 	// detach segment
	}
	return 0;
}