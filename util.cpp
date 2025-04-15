#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util.hpp"

// returns the listening socket. Terminates program on failure.
int Socket(){
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return listenfd;
}

// Terminates program on failure.
void Bind(int listenfd, struct sockaddr* sockAddr){
    if (bind(listenfd, (struct sockaddr*) sockAddr, sizeof(*sockAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

// Terminates program on failure.
void Listen(int listenfd, int n){
    if (listen(listenfd, n) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

// Returns the connection socket. Terminates program on failure.
int Accept(int listenfd, struct sockaddr* sockAddr, socklen_t* addrlen){
    int sockfd;
    if ((sockfd = accept(listenfd, (struct sockaddr*) sockAddr, addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

//starts everything up to prepare for the Accept() call. Returns the listening socket.
int networkInit(struct sockaddr_in &sockAddr, socklen_t addrlen){
    // Creating socket file descriptor
    // SOCKET()
    int listenfd = Socket();

    
    // SETSOCKOPT()
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_port = htons(PORT);

    // BIND()
    Bind(listenfd, (struct sockaddr*)&sockAddr);

    // LISTEN()
    Listen(listenfd, 3);

    return listenfd;
}

// handles a single network connection, then closes that connection on completion
int handleConnection(int sockfd){
    //loop (do 1 time)
    for(int i = 0; i < 1; ++i){
        // READ()
        char buf[MAX] = { 0 };
        ssize_t n = read(sockfd, buf, MAX - 1); 
        printf("%s\n", buf);
    
        strcpy(buf, "Hello, world\n");
    
        // SEND()
        send(sockfd, buf, strlen(buf), 0);
        printf("Hello message sent\n");
    
        // CLOSE()
        close(sockfd);
    }
}