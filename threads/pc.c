#define NPRODUCERS 10
#define NCONSUMERS 4
#define QSIZE 10

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int queue[QSIZE];
int head = 0;
int len = 0;

sem_t items;
sem_t spaces;
sem_t mutex;

void produce(int x) {
	sem_wait(&spaces);
	sem_wait(&mutex);
	int tail = (head + len) % QSIZE;
	queue[tail] = x;
	len++;
	sem_post(&mutex);
	sem_post(&items);
}

int consume(void) {
	sem_wait(&items);
	sem_wait(&mutex);
	int num = queue[head];
	head++;
	head %= QSIZE;
	len--;
	sem_post(&mutex);
	sem_post(&spaces);

	return num;
}

void *producer(void *arg) {
	int *pstart = arg;
	int start = *pstart;
	free(arg);

	for (int i = 0; i < 1000; i++) {
		int num = start + i;
		produce(num);
	}
}

void *consumer(void *arg) {
	for (;;) {
		int n = consume();
		printf("%d\n", n);
	}
}

int main(void) {
	sem_init(&items, 1, 0);
	sem_init(&spaces, 1, QSIZE);
	sem_init(&mutex, 1, 1);

	pthread_t prods[NPRODUCERS];
	pthread_t cons[NCONSUMERS];

	for (int i = 0; i < NCONSUMERS; i++) {
		pthread_create(&cons[i], NULL, consumer, NULL);
	}
	for (int i = 0; i < NPRODUCERS; i++) {
		int *p = malloc(sizeof(int));
		*p = 1000 * i;
		// thread is responsible to free memory for p
		pthread_create(&prods[i], NULL, producer, p);
	}

	for (int i = 0; i < NPRODUCERS; i++) {
		pthread_join(prods[i], NULL);
	}
	sleep(2);

	sem_destroy(&mutex);
	sem_destroy(&spaces);
	sem_destroy(&items);
}
