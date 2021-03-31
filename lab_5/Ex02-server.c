#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define MAX_SIZE 100

int main() {

	int fd, running=1;
	char string[MAX_SIZE];

	mkfifo("namedPipe", 0666);
	fd = open("namedPipe", O_RDONLY);

	while(running) {
		read(fd, string, MAX_SIZE);
		printf("Processing: %s\n", string);

		if(strcmp(string, "casa") == 0 || strcmp(string, "house") == 0) {
			printf("House detected\n");
		}
		if(strcmp(string, "exit") == 0) {
			printf("Exiting...\n");
			running = 0;
		}
	}

	close(fd);
	unlink("namedPipe");


	return 0;
}
