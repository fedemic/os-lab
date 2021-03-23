#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>


int main() {

	int i, n;
	float x, sum=0;
	int shmid, status;
	int *a;		// pointer to shared memory segment

	printf("Enter x: ");
	scanf("%f", &x);
	printf("Enter N (max allowed is 5): ");
	scanf("%d", &n);

	if(n > 5) {
		printf("Replacing your n with 5...\n");
		n = 5;
	}

	pid_t pid[n+1];

	shmid = shmget(IPC_PRIVATE, (n+1)*sizeof(float), IPC_CREAT | 0666); 	// create new shared segment between father and children
	if(shmid < 0) {
		printf("Error creating memory segment\n");
		exit(1);
	}
	a = (int *) shmat(shmid, NULL, 0);
	
	for(i=0; i<n+1; i++) {
		pid[i] = fork();

		if(pid[i] == 0) {	// child process
			a[i] = 1;
			for(int j=0; j<i; j++) {	// compute correct power
				a[i] *= x;			// child writes in shared segment
			}
			exit(0);		//child termination
		}
	}

	for(i=0; i<n+1; i++) {
		waitpid(pid[i], &status, 0);	// wait for each child to terminate
		sum += a[i]; 		// father reads shared segment
	}
	shmdt(a);			// detach segment
	shmctl(shmid, IPC_RMID, 0);		// remove shared memory id

	printf("Final result is: %.2f\n", sum);

	return 0;
}