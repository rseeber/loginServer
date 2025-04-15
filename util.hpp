#include <arpa/inet.h>

#define PORT 80
#define MAX 1024

// wrapper functions
int Socket();
void Bind(int listenfd, struct sockaddr* sockAddr);
void Listen(int listenfd, int n);
int Accept(int listenfd, struct sockaddr* sockAddr, socklen_t* addrlen);

// real stuff
int networkInit(struct sockaddr_in &sockAddr, socklen_t addrlen);
int handleConnection(int sockfd);