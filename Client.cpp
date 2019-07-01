#include "Socket.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;


int main()
{
    Socket *sock = new Socket();

    struct sockaddr_in addr = sock->createSockfd("127.0.0.1",6666);

    int sockfd = sock->getSockFd();

    sock->connect(addr);

    while(1)
    {
        char buff[128] = {0};
        cin >> buff;
        send(sockfd, buff, strlen(buff), 0);

        char recv_buff[128] = {0};
        recv(sockfd, recv_buff, 127, 0);
        cout << recv_buff << endl;
    }

    return 0;
}
