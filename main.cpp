/*
 * Project Name: Login Server (https://github.com/rseeber/loginServer)
 * Author: River Seeber (https://rseeber.github.io)
 * Libraries used: logMan by River Seeber (https://github.com/rseeber/loginManager)
*/
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util.hpp"
using namespace std;

int main(int argc, char const* argv[])
{
    int sockfd, listenfd;
    struct sockaddr_in sockAddr;
    socklen_t addrlen = sizeof(sockAddr);

    // Initialize the network listening
    listenfd = networkInit(sockAddr, addrlen);

    printf("Waiting for connections...\n");

    //loop to handle new connections until kill signal
    while(1){
        // ACCEPT()
        sockfd = Accept(listenfd, (struct sockaddr*) &sockAddr, &addrlen);
        //handle
        handleConnection(sockfd);
    }
  
    close(listenfd);
    return 0;
}
