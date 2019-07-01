#include "TcpServer.h"
#include <iostream>
#include "stdio.h"

/**********************************************************

 **********************************************************/

/*
 * 注册的写回调函数，调用TcpConnection的send()函数，该函数使用了应用层的缓冲区。
 */

//初始化的时候闯入一个EventLoop 对象和一个文件描述符
TcpServer::TcpServer(EventLoop *loop,int sockfd)
    :_loop(loop),
    _sockfd(sockfd),
    //_threadpool(new ThreadPool()),
    _acceptor(new Acceptor(loop,sockfd)),
    _eventspool(new EventLoopThreadPool(loop))
{   
    //设置Acceptor新连接的回调函数
    _acceptor->setConnectCallBack(
            std::bind(&TcpServer::newConnection,this, placeholders::_1, placeholders::_2));
}

//设置线程的个数，默认是0,当
void TcpServer::setNumsThread(int nums = 0)
{
    _numsthread = nums;
}

//开启线程池
void TcpServer::start()
{
    //_threadpool->start(_numsthread);
    _eventspool->startPool(_numsthread);
    _acceptor->listen();
}

//建立一个新的链接
void TcpServer::newConnection(struct sockaddr_in *addr, int conn)
{
    //创建一个TcpConnection对象
    //创建一个新的Channel对象,设置文件描述符上事件的回调函数
    cout << "new connect" << endl;

    //获取一个新的事件循环器,
    EventLoop *newLoop = _eventspool->getNextLoop();
    assert(newLoop != NULL);
    //printf("事件循环器的地址:0x%x\n", newLoop);

    TcpConnection *tcpconnect = new TcpConnection(newLoop, conn);
    //设置新的文件描述符的基本的回调函数
    _connections[conn] = tcpconnect;
    
    //设置读回调函数
    //tcpconnect->setReadCallBack(std::bind(&TcpServer::read, this,conn));
    tcpconnect->setReadCallBack();
    //设置写回调函数
    //tcpconnect->setWriteCallBack(_writeCallBack);
    tcpconnect->setWriteCallBack();
    //保存所有的TcpConnection的信息
    
    // 这里可以保存tcp链接的详细信息
     
    //启动开始监听该套接字
    tcpconnect->enableUpdate();    
}


/*
void TcpServer::setReadCallBack(CallBack cb)
{
    _readCallBack = cb;
}

void TcpServer::setWriteCallBack(CallBack cb)
{
    _writeCallBack = cb;
}
*/
//移除一个新的链接
void TcpServer::removeConnection()
{
    //调用connection的Channel的removeChannel 函数来删除一个监听套接字
    //首先消除所有的文件

}

void TcpServer::read(int fd)
{
    char data[128];
    cout << "接受数据" << endl;
    recv(fd, data, 128, 0);
    cout << data << endl;
}
