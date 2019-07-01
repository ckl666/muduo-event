#include "TcpConnection.h"


//在创建TcpConnection对象的时候，传入相对应的参数
TcpConnection::TcpConnection(EventLoop *loop, int sockfd)
    :_readIndex(0),
    _writeIndex(0),
    _channel(new Channel(loop, sockfd)),
    _loop(loop)
{}

//调用Channel的update开始监听新的文件描述符上的事件
void TcpConnection::enableUpdate()
{
    //该函数可以唤醒EventLoop 去添加新的文件描述符
    //设置监听的事件 
    //测试，设置读事件
    _channel->setEvents(1);
    

    _loop->runInLoop(std::bind(&Channel::update, _channel));
}

void TcpConnection::handShowDown()
{
    //先取消_channel上的所有事件
    _channel->cancleAllEvent();
    //调用Channel的removeChannel
    _channel->removeChannel(); 
}

//设置读回调函数
void TcpConnection::setReadCallBack()
{
    _channel->setReadCallBack(std::bind(&TcpConnection::recv, this,_recvBuff));
    //_channel->setReadCallBack(cb);
}

//设置写回调函数
void TcpConnection::setWriteCallBack()
{
    //注册写事件就绪函数
    _channel->setWriteCallBack(std::bind(&TcpConnection::handWrite, this));
}

//判断缓冲区的没人是否为空
bool TcpConnection::isEmptyBuffer()
{
    return _readIndex == _writeIndex;
}

///获取当前缓冲区的内容的多少
int TcpConnection::getBufferDataSize()
{
    return _writeIndex - _readIndex;
}

//将缓冲区的内容移动到缓冲区的起始位置
void TcpConnection::moveBufferData()
{
    strcpy(_buffer, _buffer+_readIndex);
}

//处理写事件就绪
void TcpConnection::handWrite()
{
    int fd = _channel->getFd();
    //将缓冲区中的数据发送
    int send_bitys = ::send(fd, _buffer+_readIndex, getBufferDataSize(),0);    
    if(send_bitys < 0)
    {
        //这里表示缓冲区已满;
    }
    else if(send_bitys < getBufferDataSize())
    {
        //没有将剩余的数据全部发送
        //注册写事件
        _readIndex += send_bitys;
        //将内容移动到缓冲区的起始位置
        moveBufferData();
    }
    else
    {
        //缓冲区没有内容的时候取消写事件
        _channel->cancleWritting();
        moveBufferData();
    }
}

//发送一个数据
//非阻塞下的发送数据函数
//这里先假定buffer缓冲区足够大
void TcpConnection::send(char *str)
{
    int fd = _channel->getFd();
    if(isEmptyBuffer())
    {
        int len = strlen(str);
        int send_bitys = ::send(fd, str, len, 0);
        if(send_bitys < len)
        {
            //将剩余的数据放到缓冲区中
            strcpy(_buffer+_writeIndex, str+send_bitys);
            //注册写事件
            _channel->enableWritting();
        }
    }
    else
    {
        //将要发送的内容放到缓冲区的后面
        strcpy(_buffer+_writeIndex, str);
    }
}

//非阻塞下的接受数据的函数
//接受的数据由用户控制
//这里的buff足够大，能够接收缓冲区中的最多的数据
int TcpConnection::recv(char *buff)
{
    //接收缓冲区
    int max_size = 4096;    //一个页的大小
    memset(buff, 0, sizeof(buff));
    int fd = _channel->getFd();
    //这里设置成非阻塞模式
    int n = ::recv(fd, buff, max_size, 0);
    if(n < 0)
    {
        perror("error");
    }
    else if(n == 0)
    {
        //对方关闭了套接字
        handShowDown();
    }
    else
    {
        char str[128] = "ok";
        printf("%s\n",buff);
        send(str);
    }
    return n;
}
