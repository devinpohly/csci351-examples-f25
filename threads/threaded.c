#include <stdio.h>
#include "csapp.h"
#include <unistd.h>

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
	struct thread_args *p = arg;
	struct thread_args args = *p;
	free(arg);

	for (;;) {
		char buf[1024];
		int bytes = Read(args.client, buf, sizeof(buf));
		if (bytes == 0) {
			break;
		}

		buf[0] = 'O';

		Write(args.client, buf, bytes);
	}

	Close(args.client);
	return NULL;
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

	int listening = Open_listenfd("9999");

	for (;;) {
		int client = Accept(listening, NULL, NULL);

		pthread_t id;
		int *p = malloc(sizeof(int));
		*p = client;
		pthread_create(&id, NULL, thread, p);
	}

	Close(listening);
	return 0;
}
