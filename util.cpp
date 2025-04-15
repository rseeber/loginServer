#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>


int Socket(){
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return listenfd;
}

void Bind(int listenfd, struct sockaddr* sockAddr){
    if (bind(listenfd, (struct sockaddr*) sockAddr, sizeof(*sockAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int listenfd, int n){
    if (listen(listenfd, n) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int Accept(int listenfd, struct sockaddr* sockAddr, socklen_t* addrlen){
    int sockfd;
    if ((sockfd = accept(listenfd, (struct sockaddr*) sockAddr, addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}