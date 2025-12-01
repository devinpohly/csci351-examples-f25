#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// used to guard access to counter
pthread_rwlock_t rw;
int counter = 0;

int heavy_calc() {
	usleep(1000);
	/* printf("%d\n", counter); */
	return 1;
}

void *thread_main(void *arg) {
	for (int i = 0; i < 1000; i++) {
		pthread_rwlock_rdlock(&rw);
		int x = heavy_calc();
		pthread_rwlock_unlock(&rw);
		pthread_rwlock_wrlock(&rw);
		counter += x;
		pthread_rwlock_unlock(&rw);
	}
}

int main(void) {
	pthread_rwlock_init(&rw, NULL);

	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		pthread_create(&threads[i], NULL, thread_main, &i);
	}

	void *ret;
	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], &ret);
	}

	printf("%d\n", counter);

	pthread_rwlock_destroy(&rw);
}
