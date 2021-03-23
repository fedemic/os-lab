#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv[]) {

	if(argc != 3) {
		printf("Parameters error. Exiting...");
		exit(1);
	}

	FILE *f;
	int i, num, sum=0, tot_num=0;

	n = atoi(*argv[2]);

	f = fopen(*argv[1], "r");
	if (f == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	for(i=0; i<n; i++) {
		fscanf(f, "%d ", &num);
		sum += num;
		tot_num++;
	}
	fclose(f);
	printf("Average of numbers: %.2f\n", (float) sum/tot_num);
}