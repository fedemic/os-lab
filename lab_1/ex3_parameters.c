#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv[]) {
	
	if(argc != 2) {
		printf("Parameters error. Exiting...");
		exit(1);
	}

	int num = atoi(*argv[1]);	
	
	if(num % 2 == 0) {
		//even number
		printf("%d is EVEN\n", num);
	}
	else {
		printf("%d is ODD\n", num);
	}
	
	return 0;
}
