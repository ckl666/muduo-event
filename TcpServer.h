#pragma once

#include "Socket.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include <functional>
#include <memory>
#include <map>

using namespace std;


class TcpServer
{
public:
    typedef std::function<void(void)> CallBack;
    //构造函数 
    TcpServer(EventLoop *loop, int sockfd);

    //设置线程池的最大数量
    void setNumsThread(int nums);

    //启动一个线程池
    void start();

    void read(int fd);
    
    //新连接到达时的处理函数
    void newConnection(struct sockaddr_in *addr, int conn);

    //断开一个链接
    void removeConnection();

    //设置读回调函数
    //void setReadCallBack(CallBack cb);
    void setReadCallBack();

    //设置写的回调函数
    //void setWriteCallBack(CallBack cb);
    void setWriteCallBack();
private:
    EventLoop *_loop;
    //当前服务器的socket
    int _sockfd;
    //线程池的个数
    int _numsthread;
     
    Acceptor *_acceptor;
    //读回调函数
    CallBack _readCallBack;
    //写回调函数
    CallBack _writeCallBack;
    EventLoopThreadPool *_eventspool;
    //这里使用智能指针
    typedef shared_ptr<TcpConnection *> TcpConnectPtr;
    //这里保存链接对象的智能指针
    std::map<int, TcpConnection*> _connections;
    //std::map<int, TcpConnection*> _connections; //这里没有使用智能指针
};
