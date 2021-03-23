#include <stdio.h>
#include <stdlib.h>

int main() {

	FILE *f;
	int i, num, sum=0, tot_num=0;

	f = fopen("random.txt", "r");
	if (f == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	for(i=0; i<100; i++) {
		fscanf(f, "%d ", &num);
		sum += num;
		tot_num++;
	}
	fclose(f);
	printf("Average of numbers: %.2f\n", (float) sum/tot_num);
}