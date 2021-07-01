#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_NUM 7
#define MAX_THREAD 3

// Global Variables
int part = -1;	// processing the thread No.

int sum = 0;
float avg = 0.0;
int num[MAX_NUM] = {90, 81, 78, 95, 79, 72, 85};
int min = 0;
int max = 0;

// Function Prototype
void *calculate (void *arg);

int main()
{
	pthread_t threads[MAX_THREAD];
	
	int i;
	for (i = 0; i < MAX_THREAD; i++) {
		pthread_create (&threads[i], NULL, &calculate, NULL);
	}
	
	for (i = 0; i < MAX_THREAD; i++) {
		pthread_join (threads[i], NULL);
	}
	
	printf ("The average value is %.0f.\n", avg);
	printf ("The minimum value is %d.\n", min);
	printf ("The maximum value is %d.\n", max);
	
	return 0;
}

/* Function calculate: can calculate many statistical values */
void *calculate (void *arg)
{
	part++; // increment when every thread starts processing
	
	int i;
	switch (part) {
		case 0:		// average
			for (i = 0; i < MAX_NUM; i++) {
				sum = sum + num[i];
			}
	
			avg = sum / MAX_NUM;
			break;
			
		case 1:		// minimum
			min = num[0];
			for (i = 0; i < MAX_NUM; i++) {
				if (num[i] < min) {
					int temp = num[i];
					num[i] = min;
					min = temp;
				}
			}
			break;
			
		case 2:		// maximum
			max = num[0];
			for (i = 0; i < MAX_NUM; i++) {
				if (num[i] > max) {
					int temp = num[i];
					num[i] = max;
					max = temp;
				}
			}
			break;
			
		default:
			break;
	}
}
