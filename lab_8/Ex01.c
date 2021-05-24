#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

// constants
#define N 16					// numbers in the array to be sorted (works only with multiple of 2)
#define MAX_NUM	100				// maximum number found in the array

// global variables
int array[N];					// array of numbers to be sorted

// structure for the threads
struct mrg_srt_data
{
	int start;
	int stop;
};

void threadGeneration(int start, int stop);	// declaration needed for *mergeSort()

// merge operation for 2 ordered sequences
void merge(int start, int stop)
{
	int i, j, k;
	int tmp;
	
	for (i = start; i <= (start+stop-1)/2; i++)			// first ordered sequence loop
	{
		for (j = (start+stop-1)/2+1; j <= stop; j++)	// second ordered sequence loop
		{
			if (array[i] > array[j])
			{
				// swap
				tmp = array[i];					
				array[i] = array[j];
				array[j] = tmp;
				
				// keep swapping in the second sequence until the correct position is found
				for (k = j; k <= stop-1; k++)	
				{					
					if (array[k] > array[k+1])
					{
						tmp = array[k];
						array[k] = array[k+1];
						array[k+1] = tmp;
					}
				}
			}
		}
	}
}

// merge-sort recursive function for threads
void *mergeSort(void *datastruct)
{	
	int N_local;
	int tmp;
	struct mrg_srt_data *data;
	
	data = (struct mrg_srt_data *)datastruct;
	N_local = data->stop - data->start + 1;
	
	if (N_local > 2)	// split
	{
		threadGeneration(data->start, data->stop);
	}
	else				// 2 element sort
	{
		tmp = array[data->stop];
		
		if (array[data->start] > array[data->stop])
		{
			array[data->stop] = array[data->start];
			array[data->start] = tmp;
		}
	}
	
	// n-element sort	
	merge(data->start, data->stop);
		
	pthread_exit(NULL);
}

// function to generate 2 merge-sort threads
void threadGeneration(int start, int stop)
{
	// initial data
	pthread_t tid[2];
	struct mrg_srt_data threads_data[2];
	int rc;
	
	// threads generation
	threads_data[0].start = start;
	threads_data[0].stop = (start+stop-1)/2;
	
	rc = pthread_create(&tid[0], NULL, mergeSort, (void *)&threads_data[0]);
	if (rc != 0)
	{
		printf("Error in generating thread with ID: %lu\n", tid[0]);
		exit(-1);
	}
	
	threads_data[1].start = (start+stop-1)/2+1;
	threads_data[1].stop = stop;
	
	rc = pthread_create(&tid[1], NULL, mergeSort, (void *)&threads_data[1]);
	if (rc != 0)
	{
		printf("Error in generating thread with ID: %lu\n", tid[0]);
		exit(-1);
	}
	
	// threads join
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);	
}

int main()
{
	int i;
	
	// array generation and print
	srand(getpid());
	
	printf("Initial array\n");
	
	for (i = 0; i < N; i++)
	{
		array[i] = rand()%MAX_NUM;
		printf("array[%d]:\t %d\n", i, array[i]);
	}
	
	// first call for the recursive thread generation and merge-sort
	threadGeneration(0, N-1);
	
	// last merge operation for the 2 subsequences of lenght N/2
	merge(0, N-1);
	
	// print ordered array
	printf("\nOrdered array\n");
	for (i = 0; i < N; i++)
	{
		printf("array[%d]:\t %d\n", i, array[i]);
	}
	
	return 0;
}
