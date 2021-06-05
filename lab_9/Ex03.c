#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>

#define MAX_FILENAME 30
#define MAX_OPERATIONS 100

int main() {

	int *SV; 			// shared variable
	int SV_shmid, sem_shmid;			//shared memory IDs
	int random_num;

	pid_t p1;
	sem_t sem;

	// shared memory initializations
	if(SV_shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666) == -1) {
		printf("Error creating SV shared memory. Exiting...\n");
	}
	if(sem_shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666) == -1) {
		printf("Error creating semaphore shared memory. Exiting...\n");
	}

	// memory attach
	SV = (int *) shmat(SV_shmid, NULL, 0);
	sem = (int *) shmat(sem_shmid, NULL, 0);

	*SV = -1;
	sem_init(&sem, 1, 1);

	p1 = fork(); 		// first child generation

	if(p1 == 0) {		// child (p1) process

		pid_t pid;
		srand(getpid()); 		// PID used as rand seed

		while(1) {

			random_num = rand() % 3;
			sleep(random_num);

			pid = fork();

			if(pid == 0) {		// child of p1 process
				srand(getpid());
				random_num = rand() % 5;
				sleep(random_num);

				sem_wait(&sem);

				// CS begin
				if(*SV == -1) {
					// in this condition signal semaphore and CS end
					sem_post(&sem);
				}
				else {
					printf("SV = %d\n", *SV);
					*SV = -1;
					// CS end
					sem_post(&sem);
					exit(0);
				}
			}
		}

	}
	else {				// parent process

		srand(getpid());		// random seed as parent PID
		while(1) {
			random_num = rand() % 10;
			sleep(random_num);

			sem_wait(&sem);
			// CS begin
			*SV = random_num;
			// CS end
			sem_post(&sem);

		}
	}


	// detach and remove memory
	shmdt(SV);
	shmctl(SV_shmid, IPC_RMID, 0);
	shmdt(&sem);
	shmctl(sem_shmid, IPC_RMID, 0);

	// delete semaphore
	sem_destroy(&sem);

	return 0;

}
