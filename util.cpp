#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "util.hpp"

using namespace std;

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
        //printf("[CLIENT]: '%s'\n", buf);
    
        //read request, generate response
        string response = generateResponse(buf);

        // SEND()
        int cnt = 0;
        while(cnt < response.length()){
            cnt += send(sockfd, response.c_str(), response.length(), 0);
        }
        //printf("[SERVER]: '%s'\n", response.c_str());
    
        // CLOSE()
        close(sockfd);
    }
    return 0;
}

//returns string of the response for the given request.
//parse the input for the requested file, read the file, and return its contents
string generateResponse(char* req){
    //use stringstream so we can read formatted input from the req
    stringstream msg;
    msg.str(req);

    //read the first line of request header
    string method, resource, protocol;
    msg >> method >> resource >> protocol;
    
    if(strcmp(resource.c_str(), "/") == 0){
        resource.assign("/index.html");
    }

    //open the requested file
    ifstream f;
    f.open("./web"+resource);
    //if we can't properly open it
    if(!f.good()){
        //404 error
        resource = "/404.html";
        f.open("./web"+resource);
    }

    //write the file to response
    string response, temp;

    while(getline(f, temp)){
        response += temp+"\n";
    }

    // return the file
    return response;
}