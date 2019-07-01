#pragma once

#include "EventLoop.h"
#include <vector>
#include <pthread.h>
#include "ThreadPool.h"

class EventLoopThreadPool
{
public:
    typedef std::vector<EventLoop *> EventloopList;

    EventLoopThreadPool(EventLoop *baseloop);

    //在析构函数中释放线程池
    ~EventLoopThreadPool();

    void startPool(int numsEventLoops);

    EventLoop *getNextLoop();

    EventLoop *getHashLoop();

    EventloopList getAllLoop();

    //绑定线程池中的线程执行的函数
private:
    EventloopList _eventloops;
    ThreadPool _threadpool;

    int _next;
    EventLoop *_baseloop;
};
