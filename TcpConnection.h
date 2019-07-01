#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"


//该对象用于发送数据
//
//接收有没有必要设置一个缓冲区？这个要看应用层对数据的处理
//如果应用层只有在数据完整的情况下才会处理数据的话，需要一个缓冲区
//如果应用层对数据是否完整要求不高的话，例如，只是保存接收到的数据
//在数据的到达时间不影响具体的操作，则可以不设置缓冲区
//
//如何知道接收到的数据是否完整？
//
//
//#include <vector>

//缓冲区的大小64k
#define MAX_BUFF 65536

class TcpConnection
{
public:
    typedef std::function<void(void)> CallBacl;
    TcpConnection( EventLoop *loop, int sockfd);
    
    //发送数据
    void send(char *str);
    
    int getBufferDataSize();

    void moveBufferData();
    //当可以发送数据的时候，先判断缓冲区中是否还有内容
    //如果有，先发送缓冲区的内容，如果缓冲区的内容能够发完
    //则再发送新的数据，否则，将新的数据和未发完的数据都放入
    //缓冲区中，再套接字上注册一个写事件监听的函数，当有写事件
    //产生的时候，调用注册的回调函数

    void handShowDown();

    //处理写事件就绪函数
    void handWrite();

    void setReadCallBack();

    void setWriteCallBack();

    //判断缓冲区是否为空
    bool isEmptyBuffer();
    
    int recv(char *buff);

    //开始监听新的文件描述符上的事件
    void enableUpdate();

private:
    Channel *_channel;
    EventLoop *_loop;
    char _buffer[MAX_BUFF];
    unsigned int _readIndex;
    unsigned int _writeIndex;
    char _recvBuff[4096];   //一个页面的接收缓冲区
};
