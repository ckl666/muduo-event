#include "Acceptor.h"


Acceptor::Acceptor(EventLoop *loop, int sockfd)
    :_loop(loop),
    _sockfd(sockfd),
    _channel(new Channel(loop, sockfd)),
    _acceptSocket(new Socket(sockfd)),
    _listening(false)
{
    //设置回调函数
    _channel->setReadCallBack(std::bind(&Acceptor::handRead, this));
}


void Acceptor::listen()
{
    if(_listening)
    {
        return ;
    }
    else
    {
        _listening = true;
        //开始监听读事件
        _channel->enableReading();
        _acceptSocket->listen(MAX_LISTEN);
    }
}


//
void Acceptor::handRead()
{
    struct sockaddr_in *addr = new struct sockaddr_in;
    //这里直接调用Socket的accept 函数进行接收
    int conn = _acceptSocket->accept(*addr);
    if(conn < 0)
    {
        perror("error\n");
    }
    else
    {
        //创建一个新的链家交给tcpServer处理
        //function<void(void)> f = std::bind(_newConnect, addr, conn);
        _newConnect(addr, conn);
    }
}
