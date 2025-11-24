#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

// used to guard access to counter
sem_t lock;
int counter = 0;

int heavy_calc() {
	usleep(1000);
	return 1;
}

void *thread_main(void *arg) {
	for (int i = 0; i < 1000; i++) {
		int x = heavy_calc();
		sem_wait(&lock);
		counter += x;
		sem_post(&lock);
	}
}

int main(void) {
	sem_init(&lock, 1, 1);

	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		pthread_create(&threads[i], NULL, thread_main, &i);
	}

	void *ret;
	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], &ret);
	}

	printf("%d\n", counter);

	sem_destroy(&lock);
}
