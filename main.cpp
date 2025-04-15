#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util.hpp"

#define PORT 80
#define MAX 1024

int main(int argc, char const* argv[])
{
    int sockfd, listenfd;
    struct sockaddr_in sockAddr;
    socklen_t addrlen = sizeof(sockAddr);

    // Creating socket file descriptor
    // SOCKET()
    listenfd = Socket();

    
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

    // ACCEPT()
    sockfd = Accept(listenfd, (struct sockaddr*) &sockAddr, &addrlen);
  
    //loop that only happens once (for now)
    do{
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
    } while(0);
  
    close(listenfd);
    return 0;
}
