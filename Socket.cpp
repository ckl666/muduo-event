#include "Socket.h"

//socket 封装

Socket::Socket(int sockfd)
    :_sockfd(sockfd)
{}

Socket::Socket()
{}

int Socket::listen(int n)
{
    return ::listen(_sockfd, n);
}

int Socket::accept(struct sockaddr_in &addr)
{
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    int c = ::accept(_sockfd, (struct sockaddr *)&addr, &len);
    assert(c != -1);
    return c;
}

int Socket::connect(struct sockaddr_in &addr)
{
    int res = ::connect(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(res != -1);
    return res;
}

int Socket::bind(struct sockaddr_in &addr)
{
    int res = ::bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(res != -1);
    return res;
}

struct sockaddr_in Socket::createSockfd(string strIp, int port)
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    
    //在外面判断当前的sockfd是否合法在使用
    _sockfd = sockfd;

    struct sockaddr_in addr;
    
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(strIp.c_str());

    return addr;
}

int Socket::getSockFd()
{
    return _sockfd;
}
