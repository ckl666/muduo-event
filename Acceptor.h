#pragma once

#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"

/*
 *  首先：设置接收到新的连接后的回调函数
 *  当有read事件到达的时候，需要一个回调函数来处理连接请求
 *  产生的新的文件描述符交给tcpServer
 *  在tcpServer的中创建一个新的tcpConnection
 */

#define MAX_LISTEN 5

class Acceptor
{
public:
    typedef std::function<void(struct sockaddr_in*, int)> newConnectCallBack;

    Acceptor(EventLoop *loop, int sockfd);

    //当有新的连接的时候，处理新链家
    void handRead();

    void listen();

    //设置回调函数
    void setConnectCallBack(newConnectCallBack cb)
    {
        _newConnect = cb;
    }

    Channel *getChannel()
    {
        return _channel;
    }

private:
    //Channel 里面存放服务器的socket 
    Channel *_channel;
    EventLoop *_loop;

    //存放连接的客户端的地址信息
    struct sockaddr_in _newaddr;

    //socket 对象用来处理连接
    Socket *_acceptSocket;
    //新连接到来时的回调函数,传入一个客户端的地址
    newConnectCallBack _newConnect;

    //存放服务端的套接字
    int _sockfd;
    bool _listening;
};
