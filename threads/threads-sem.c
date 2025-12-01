#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t signal;

struct thread_args {
	int x;
	int y;
};

void * thread_main(void *arg) {
	struct thread_args *args = arg;
	struct thread_args a = *args;
	sem_post(&signal);
	
	usleep(rand() % 1000000);
	printf("%d + %d = %d!\n", a.x, a.y, a.x + a.y);

	pthread_exit(NULL);
	/* return NULL; */
}

int main(void) {
	srand(time(NULL));
	sem_init(&signal, 1, 0);

	printf("hello there\n");
	void *ret;

	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		struct thread_args a;
		a.x = i;
		a.y = i * i;
		// child thread should signal once args are read
		pthread_create(&threads[i], NULL, thread_main, &a);
		sem_wait(&signal);
	}

	/* printf("waiting\n"); */

	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], &ret);
	}

	printf("done\n");
	sem_destroy(&signal);

	return 0;
}
