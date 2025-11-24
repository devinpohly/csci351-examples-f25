#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// used to guard access to counter
pthread_mutex_t lock;
int counter = 0;

int heavy_calc() {
	usleep(1000);
	return 1;
}

void *thread_main(void *arg) {
	for (int i = 0; i < 1000; i++) {
		int x = heavy_calc();
		pthread_mutex_lock(&lock);
		counter += x;
		counter += x;
		pthread_mutex_unlock(&lock);
	}
}

int main(void) {
	pthread_mutex_init(&lock, NULL);

	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		pthread_create(&threads[i], NULL, thread_main, &i);
	}

	void *ret;
	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], &ret);
	}

	printf("%d\n", counter);

	pthread_mutex_destroy(&lock);
}
