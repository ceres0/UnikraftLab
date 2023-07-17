// Copy previous work item's main here after it is completed and update
// with your encoding function call
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define BUFLEN 100
#define LISTEN_PORT 1234

// Place this somewhere in `lwip`
// void rot13(char *msg) {
//     for (; *msg; msg++) {
//         if (*msg == '\n') {
//             continue;
//         }

//         if ((*msg > 'm' && *msg < 'z') || (*msg > 'M' && *msg < 'Z')) {
//             *msg = *msg - 13;

//         } else {
//             *msg = *msg + 13;
//         }
//     }
// }

int receive_and_send(int client_recvfd, int client_sendfd)
{
	char buf[BUFLEN];
	int bytes_send = 0;
	int bytes_received = 0;

	memset(buf, 0, BUFLEN);

	/* TODO: recv() buffer into buf; */
	/* bytes_received = ...*/
	bytes_received = recv(client_recvfd, buf, BUFLEN, 0);

	if (bytes_received < 0) {
		fprintf(stderr, "bytes_received recv");
		return -1;
	}

    rot13(buf);

	/* TODO: send() buf back */
	/* bytes_send = ... */	
	bytes_send = send(client_sendfd, buf, BUFLEN, 0);

	if (bytes_send < 0) {
		fprintf(stderr, "bytes_send send");
		return -1;
	}


	return bytes_received;
}

int main(int argc, char* argv[])
{
	int listen_fd = -1;
	int err;
	struct sockaddr_in serv_addr;

	/* TODO: open socket fd; */
	/* listen_fd = ... */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_fd < 0) {
		fprintf(stderr, "socket");
		return 1;
	}

	int enable = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
		fprintf(stderr, "setsocketopt");
		return 1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(LISTEN_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	/* TODO: bind() the socket; */
	/* err =  ... */
	err = bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	if (err < 0) {
		fprintf(stderr, "bind");
		return 1;
	}

	struct sockaddr_in client_addr;

	int bytes_received;
	int conn_fd = -1;
	socklen_t socket_len = sizeof(struct sockaddr_in);

	listen(listen_fd, 1);

	// printf("Listening on port %d...\n", LISTEN_PORT);

	/* TODO: accept() new connection; */
	/* conn_fd = ... */
	conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &socket_len);

	// printf("Connect success!\n");

	if (conn_fd < 0) {
		fprintf(stderr, "conn_fd accept");
		return 1;
	}

	do {
		bytes_received = receive_and_send(conn_fd, conn_fd);
	} while (bytes_received > 0);

	/* TODO: close file descriptors */
	close(conn_fd);
	close(listen_fd);

	return 0;
}
