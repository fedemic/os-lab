#include <stdio.h>

int main() {
	int num;

	printf("Insert an integer number:\n");
	scanf("%d", &num);
	if(num % 2 == 0) {
		//even number
		printf("%d is EVEN\n", num);
	}
	else {
		printf("%d is ODD\n", num);
	}
}
