#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

// shared buffer, adjust this size and notice the interleaving
#define BSIZE 3
typedef struct
{
	char buf[BSIZE];
	int occupied;
	int nextin, nextout;
	pthread_mutex_t mutex;
	pthread_cond_t more;
	pthread_cond_t less;
} buffer_t;
buffer_t buffer;

// running flag for thread termination
int done = 0;
int sleep_time;

// threads, and pointers to associated functions
void *producerFunction(void *);
void *consumerFunction(void *);
pthread_t producerThread;
pthread_t consumerThread;

int main(int argc, char *argv[])
{

	// check that arguments make sense, print error message if they do not.
	if (argc != 2)
	{
		printf("Usage: ./matrix [sleep_time]");
		exit(1);
	}

	// initialize sleep_time and buffer variables

	sleep_time = atoi(argv[1]);

	if (sleep_time == 0)
	{
		printf("Error: Argument is not an integer or invalid number\n");
		exit(1);
	}

	buffer.occupied = 0;
	buffer.nextin = 0;
	buffer.nextout = 0;
	pthread_mutex_init(&buffer.mutex, NULL);
	pthread_cond_init(&buffer.more, NULL);
	pthread_cond_init(&buffer.less, NULL);

	// create 2 threads (consumer and producer)
	pthread_create(&producerThread, NULL, producerFunction, NULL);
	pthread_create(&consumerThread, NULL, consumerFunction, NULL);

	// join the two threads
	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);

	printf("main() exiting properly, both threads have terminated. \n");
	return (0);
}

void *producerFunction(void *parm)
{
	printf("producer starting... \n");

	// objects to produce, place in buffer for the consumer
	char item[] = "More than meets the eye!";

	int i;
	for (i = 0;; i++)
	{

		// done producing when end of null terminated string
		if (item[i] == '\0')
			break;

		// acquire lock
		int isSuccess = pthread_mutex_lock(&buffer.mutex);
		if (isSuccess == 0)
			printf("producer has the lock. \n");

		// debug info
		if (buffer.occupied >= BSIZE)
			printf("producer waiting, full buffer ... \n");

		// use the conditional variable to make sure the buffer isn't full
		while (buffer.occupied >= BSIZE)
		{
			pthread_cond_wait(&buffer.less, &buffer.mutex);
		}

		// add to the buffer
		buffer.buf[buffer.nextin] = item[i];
		buffer.nextin = (buffer.nextin + 1) % BSIZE;
		buffer.occupied++;

		// debug info
		printf("producing object number: %i [%c]\n", i, item[i]);

		// signal the producer, release the lock
		pthread_cond_signal(&buffer.more);
		pthread_mutex_unlock(&buffer.mutex);

		// tell consumer we are no longer producing more items
		// by setting the done flag if this is the last element
		if (item[i + 1] == '\0')
			done = 1;

		// impose a delay to show mutual exclusion
		sleep(sleep_time);
	}
	printf("producer exiting. \n");
	pthread_exit(0);
}

void *consumerFunction(void *parm)
{
	printf("consumer starting \n");

	char item;
	int i;
	for (i = 0;; i++)
	{
		// is the producer still running?
		if (done == 1)
			break;

		// acquire lock
		int isSuccess = pthread_mutex_lock(&buffer.mutex);
		if (isSuccess == 0)
			;
		printf("consumer has the lock. \n");

		// debug info
		if (buffer.occupied == 0)
			printf("consumer waiting, empty buffer ... \n");

		// use the conditional variable to make sure the buffer isn't empty
		while (buffer.occupied == 0)
		{
			pthread_cond_wait(&buffer.more, &buffer.mutex);
		}

		// consume from buffer by displaying to the terminal
		item = buffer.buf[buffer.nextout];
		buffer.nextout = (buffer.nextout + 1) % BSIZE;
		buffer.occupied--;
		printf("consuming object number %i [%c]\n", i, item);

		// signal the producer, and release the lock
		pthread_cond_signal(&buffer.less);
		pthread_mutex_unlock(&buffer.mutex);
	}
	printf("consumer exiting. \n");
	pthread_exit(0);
}
