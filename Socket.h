#pragma once

#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

using namespace std;
/*
 * 此类之位上层提供服务
 * 接收需要的参数，提供上层需要的结果
 *
 * 如果要实现p2p模式，必须要保存各个客户端的地址
 * 
 * p2p模式下可以将
 */

class Socket
{
public:
    //构造函数
    //创建一个socket的时候，
    //sockaddr()
    //  ip
    //  port
    //  协议
    Socket(int sockfd);

    //默认构造函数
    Socket();
    
    //sockfd
    //&sockaddr
    int bind(struct sockaddr_in &addr);
    
    //sockfd
    //nums;
    int listen(int n);

    //&sockaddrv
    //sockfd
    int accept(struct sockaddr_in &addr);

    //&sockaddr
    //sockfd
    int connect(struct sockaddr_in &addr);

    struct sockaddr_in createSockfd(string strIp, int port);

    int getSockFd();
private:
    //连接套接字
    int _sockfd;
};
