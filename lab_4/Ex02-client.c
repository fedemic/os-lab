#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE 1000

struct msgbuf {
	long type;
	char data[MAX_SIZE];
};

int main() {

	key_t key;
	int msg_id, running=1;
	char command[MAX_SIZE];

	struct msgbuf mymsg;
	mymsg.type = 1;		// initilization of msg type


	system("touch file_for_unique_key.txt");		// file to create unique key
	key = ftok("file_for_unique_key.txt", 'F');
	if(key == -1) {
		perror("Error generating key: ");
		exit(1);
	}

	msg_id = msgget(key, IPC_CREAT | 0666);			// create queue
	while(running) {
		printf("Enter a command: ");
		fgets(command, MAX_SIZE, stdin);
		command[strcspn(command, "\n")] = 0;	// remove newline added by fgets function
		strcpy(mymsg.data, command);			// copy command into message data

		if(msgsnd(msg_id, &mymsg, strlen(mymsg.data)+1, 0) == -1) {		// send message through queue
			perror("Error in sending message to queue: ");
			exit(1);
		}

		if(strcmp(command, "exit") == 0) {
			running = 0;
		}
	}

	return 0;

}