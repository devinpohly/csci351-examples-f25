#include <unistd.h>
#include <stdio.h>
#include "csapp.h"

int main(void) {
#if 0
	int fd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	/* uint32_t addr_int = htonl(0x0a1651c3); */
	/* addr.sin_addr.s_addr = addr_int; */
	inet_aton("10.22.81.195", &addr.sin_addr);
	addr.sin_port = htons(9999);

	Connect(fd, (struct sockaddr *) &addr, sizeof(addr));
#endif

	int fd = Open_clientfd("cslab02.wheaton.edu", "9999");

	char buf[1024];
	strcpy(buf, "hello world\n");
	Write(fd, buf, 12);

	int bytes = Read(fd, buf, sizeof(buf));
	Write(STDOUT_FILENO, buf, bytes);

	Close(fd);
	return 0;
}
