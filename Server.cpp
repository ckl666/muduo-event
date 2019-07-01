#include "TcpServer.h"
#include "TcpConnection.h"
#include "Socket.h"
#include <iostream>

//创建TcpServer 对象
using namespace std;

class Ser
{
public:
    /*
    void read(int fd)
    {
        cout << "调用read回调函数" << endl;
    }
    */

    void fun()
    {
        //创建一个事件循环器
        EventLoop *loop = new EventLoop();

        //创建套接字并且绑定
        Socket *sock = new Socket();    
        struct sockaddr_in addr = sock->createSockfd("127.0.0.1",6666);
        sock->bind(addr);
        
        //获取创建的套接字
        int sockfd = sock->getSockFd();
        cout << sockfd << endl;
        TcpServer *ser = new TcpServer(loop, sockfd);
        //设置读的回掉函数
        //ser->setReadCallBack(std::bind(&Ser::read,this));

        ser->setNumsThread(4);
        ser->start();
        //sleep(10);
        //当其他的线程在循环事件的时候，主线程负责的事情
        //主线程负责分配一个事件循环器，
        //主线程监听server套接字,当有新的链接到达的时候
        //生成Channel的时候，会要求传入一个EventLoop,
        //所有在生成Channel 之前，要从池中选择一个事件循环器
        //
        loop->loop();
    }
};

int main()
{
    Ser ser;
    ser.fun();
    return 0;
}
