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
	fd = open("namedPipe", O_WRONLY);

	while (running) {
		printf("Enter a string: ");
		fgets(string, MAX_SIZE, stdin);
		string[strcspn(string, "\n")] = 0;	// remove newline added by fgets function

		write(fd, string, strlen(string)+1);		// write string into the named pipe

		if(strcmp(string, "exit") == 0) {		// exit condition
			running = 0;
		}
	}

	close(fd);


	return 0;
}
