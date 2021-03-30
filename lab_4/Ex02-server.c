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
	int msg_id, running = 1;
	char command[MAX_SIZE];

	struct msgbuf mymsg;

	key = ftok("file_for_unique_key.txt", 'F');
	if(key == -1) {
		perror("Error generating key: ");
		exit(1);
	}

	msg_id = msgget(key, IPC_CREAT | 0666);			// get queue
	while(running) {

		if(msgrcv(msg_id, &mymsg, MAX_SIZE, 0, 0) == -1) {		// receive message from queue
			perror("Error in receiving message from queue: ");
			exit(1);
		}

		if(strcmp(mymsg.data, "exit") != 0) {
			system(mymsg.data);
		}
		else {
			running = 0;
			if(msgctl(msg_id, IPC_RMID, 0) == -1) {			// if exit remove the used queue
				perror("Error removing queue: ");
				exit(1);
			}
		}
		
	}

	return 0;

}