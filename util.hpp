#include <arpa/inet.h>

int Socket();
void Bind(int listenfd, struct sockaddr* sockAddr);
void Listen(int listenfd, int n);
int Accept(int listenfd, struct sockaddr* sockAddr, socklen_t* addrlen);