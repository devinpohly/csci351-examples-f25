#include <stdio.h>
#include "csapp.h"
#include <unistd.h>

int main(void) {
#if 0
	int fd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9998);
	inet_aton("10.22.81.195", &addr.sin_addr);

	Bind(fd, (struct sockaddr *) &addr, sizeof(addr));

	Listen(fd, 10);
#endif
	int fd = Open_listenfd("9999");

	for (;;) {
		int client = Accept(fd, NULL, NULL);

		for (;;) {
			char buf[1024];
			int bytes = Read(client, buf, sizeof(buf));
			if (bytes == 0) {
				break;
			}

			buf[0] = 'O';

			Write(client, buf, bytes);
		}

		Close(client);
	}

	Close(fd);
	return 0;
}
