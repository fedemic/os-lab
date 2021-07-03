#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>

struct data {
	sem_t sem_r, sem_w;
	int num, result;		// num: from parent to child;	result: from child to parent
};

int main() {
	pid_t p1, p2;
	int fid1, fid2;
	struct data *data1, *data2;
	FILE *fp;
	int read_num;

	int value;

	fid1 = shmget(IPC_PRIVATE, sizeof(struct data), IPC_CREAT | 0666);
	if(fid1 == -1) {
		perror("Error creating memory segment 1.\n");
		exit(-1);
	}

	fid2 = shmget(IPC_PRIVATE, sizeof(struct data), IPC_CREAT | 0666);
	if(fid2 == -1) {
		perror("Error creating memory segment 2.\n");
		exit(-1);
	}

	data1 = (struct data *) shmat(fid1, NULL, 0);
	if(data1 == (struct data *)-1) {
		perror("Error attaching memory 1.\n");
		exit(-1);
	}

	data2 = (struct data *) shmat(fid2, NULL, 0);
	if(data2 == (struct data *)-1) {
		perror("Error attaching memory 2.\n");
		exit(-1);
	}

	sem_init(&data1->sem_r, 1, 0);
	sem_init(&data1->sem_w, 1, 1);		// only data1->sem_r is used to sync children and parent to proceed with next number; data2->sem_w is unused
	sem_init(&data2->sem_r, 1, 0);

	p1 = fork();
	if(p1 < 0) {
		perror("Error creating first child.\n");
		exit(-1);
	}
	else if(p1 == 0) {		// first child that works on positive numbers

		while(data1->num >= 0) {		// condition to end the child process is a negative number sent from parent

			sem_wait(&data1->sem_r);		// wait for parent to send new number

			if(data1->num > data1->result) {		// if new max is found then update
				data1->result = data1->num;
			}

			sem_post(&data1->sem_w);			// signal parent to continue execution and read next number
		}

		exit(0);
	}

	p2 = fork();
	if(p2 < 0) {
		perror("Error creating second child.\n");
		exit(-1);
	}
	else if(p2 == 0) {		// second child that works on negative numbers

		while(data2->num <= 0) {		// condition to end the child is a positive number passed from the parent

			sem_wait(&data2->sem_r);

			if(data2->num < data2->result) {		// if new min is found then update
				data2->result = data2->num;
			}

			sem_post(&data1->sem_w);			// signal on data1->sem_w to let parent read next number
		}

		exit(0);
	}

	else if(p1 > 0 && p2 > 0) {		// parent process
		fp = fopen("numbers.dat", "r");
		if(fp == NULL) {
			perror("Error opening file.\n");
			exit(-1);
		}

		while(!feof(fp)) {

			sem_wait(&data1->sem_w);		// wait for children to let parent continue execution

			fscanf(fp, "%d", &read_num);

			if(read_num > 0) {		// update struct of first child and signal it
				data1->num = read_num;
				sem_post(&data1->sem_r);
			}
			else if(read_num < 0) {		// call second child
				data2->num = read_num;
				sem_post(&data2->sem_r);
			}
		}

		// once the file has ended put condition to terminate children
		data1->num = -1;
		sem_post(&data1->sem_r);
		data2->num = 1;
		sem_post(&data2->sem_r);

		waitpid(p1, NULL, 0);
		waitpid(p2, NULL, 0);

	}

	fclose(fp);

	printf("MAX: %d\tMIN: %d\n", data1->result, data2->result);

	sem_destroy(&data1->sem_r);
	sem_destroy(&data1->sem_w);
	sem_destroy(&data2->sem_r);
	sem_destroy(&data2->sem_w);

	shmdt(data1);
	shmdt(data2);

	shmctl(fid1, IPC_RMID, 0);
	shmctl(fid2, IPC_RMID, 0);

	return 0;
}