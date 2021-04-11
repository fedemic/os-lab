#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define N 10
#define NAME "/sh_mem"
#define SIZE (N * sizeof(int))

int main() {

	pid_t pid;
	int fd_parent, fd_child;
	int *data;
	int *rx_data;
	int num = 1, read_num = 1;
	FILE *f;


	pid = fork();
	if(pid < 0) {
		printf("Error creating child process\n");
		exit(1);
	}
	else if(pid == 0) {	// child process
		sleep(0.5);
		fd_child = shm_open(NAME, O_RDWR, 0666);
		if(fd_child < 0) {
			perror("shm_open child: ");
			exit(1);
		}

		rx_data = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd_child, 0);

		while(read_num != 0) {
			memcpy(&read_num, rx_data, sizeof(int));
			printf("Child read: %d\n", read_num);
			sleep(1);
		}
		munmap(rx_data, SIZE);
		shm_unlink(NAME);

	}
	else {		// parent process
		
		fd_parent = shm_open(NAME, O_CREAT | O_RDWR, 0666);
		if(fd_parent < 0) {
			perror("shm_open parent: ");
			exit(1);
		}
		ftruncate(fd_parent, SIZE);

		data = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_parent, 0);
		if(data < 0) {
			perror("mmap parent: ");
			exit(1);
		}

		f = fopen("number.dat", "r");
		
		if (f == NULL) {
			printf("Error opening file\n");
			exit(1);
		}

		while(num != 0) {
			fscanf(f, "%d", &num);
			memcpy(data, &num, sizeof(int));
			printf("Number read: %d\n", num);
			sleep(1);
		}
		munmap(data, SIZE);
		close(fd_parent);
	}

	return 0;
}