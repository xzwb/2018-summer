#define _MY_SOCKET_C

#include "my_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "my_pack.h"
#include "my_err.h"

int my_accept_seve(void) {
    int sock_fd;
    int ret;
    int len;
    int opt;
    int i;
    struct sockaddr_in seve;
    opt = 1;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        my_err("socket",__LINE__);
    }
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&seve, 0, sizeof(struct sockaddr_in));
    seve.sin_family = AF_INET;
    seve.sin_port = htons(PORT);
    seve.sin_addr.s_addr = htonl(INADDR_ANY);
    if (fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFD, 0) | O_NONBLOCK) < 0) {
        my_err("fcntl",__LINE__);
    }
    if (bind(sock_fd, (struct sockaddr*)&seve, sizeof(struct sockaddr_in)) < 0) {
        my_err("bind",__LINE__);
    }
    if (listen(sock_fd, 15) < 0) {
        my_err("listen", __LINE__);
    }
     
    return sock_fd;
}

int my_accept_cli(void) {
    int sock_fd;
    int ret;
    struct sockaddr_in seve;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        my_err("socket", __LINE__);
    }
    memset(&seve, 0, sizeof(struct sockaddr_in));
    seve.sin_family = AF_INET;
    seve.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &seve.sin_addr);
    if (connect(sock_fd, (struct sockaddr*)&seve, sizeof(struct sockaddr_in)) < 0) {
        my_err("connect", __LINE__);
    }
    
    return sock_fd;
}

