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
	char *str, command[MAX_LENGTH];
	int quit = 0;


	key = ftok("/tmp", 'A');				// generating unique key for shared segment
	shmid = shmget(key, MAX_LENGTH, IPC_CREAT | 0777);

	if(shmid < 0) {
		printf("Error creating shared segment\n");
		exit(1);
	}

	while(quit == 0) {
		str = (char *)shmat(shmid, NULL, 0);		// attach segment
		if(str == (char *)-1) {
			printf("Error in shmat server\n");
			exit(1);
		}

		strcpy(command, str);
		shmdt(str); 					// detach segment

		if(strcmp(command, "exit") != 0) {
			system(command);			// execute command different than exit
		}
		else {							// exit condition
			quit = 1;
			shmctl(shmid, IPC_RMID, NULL);		// remove shared segment when exiting program
		}
		sleep(10);						// execute a command every 5 seconds
	}

	return 0;
}