#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void * thread_main(void *arg) {
	printf("hi!\n");

	pthread_exit(NULL);
	/* return NULL; */
}

int main(void) {
	printf("hello there\n");

	pthread_t thread;
	pthread_create(&thread, NULL, thread_main, NULL);

	sleep(1);

	printf("waiting\n");

	void *ret;
	pthread_join(thread, &ret);

	printf("done\n");

	return 0;
}
