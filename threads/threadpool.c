#include <stdio.h>
#include "csapp.h"
#include <unistd.h>

int queue[10];
int head = 0;
int len = 0;

sem_t items;
sem_t spaces;
sem_t mutex;

void produce(int x) {
	sem_wait(&spaces);
	sem_wait(&mutex);
	int tail = (head + len) % 10;
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
	head %= 10;
	len--;
	sem_post(&mutex);
	sem_post(&spaces);

	return num;
}

void handle(int sig) {
	int pid;
	while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		fprintf(stderr, "reaped %d\n", pid);
	}
	if (pid < 0 && errno != ECHILD) {
		perror("waitpid");
		exit(1);
	}
}

struct thread_args {
	int client;
	int listening;
};

void *thread(void *arg) {
	for (;;) {
		int fd = consume();

		for (;;) {
			char buf[1024];
			int bytes = Read(fd, buf, sizeof(buf));
			if (bytes == 0) {
				break;
			}

			buf[0] = 'O';

			Write(fd, buf, bytes);
		}

		Close(fd);
	}
}

int main(void) {
#if 0
	int listening = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9998);
	inet_aton("10.22.81.195", &addr.sin_addr);

	Bind(listening, (struct sockaddr *) &addr, sizeof(addr));

	Listen(listening, 10);
#endif
	Signal(SIGCHLD, handle);

	for (int i = 0; i < 10; i++) {
		pthread_t id;
		pthread_create(&id, NULL, thread, NULL);
	}

	int listening = Open_listenfd("9999");

	for (;;) {
		int client = Accept(listening, NULL, NULL);
		produce(client);
	}

	Close(listening);
	return 0;
}
