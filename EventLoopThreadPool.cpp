#include "EventLoopThreadPool.h"

/*
 * 创建一个startPool函数未实现
 */

//这里要求任务的数量与线程的数量一样多，
//如果任务数量多于线程的数量，
//可能导致所有的线程都在监听文件描述符上的事件而无法处理多余的任务
//这里的numsEventLoops 就是线程的数量，也是任务队列的的最大任务数

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop)
    :_next(0),
    _baseloop(baseloop)
{}

EventLoopThreadPool::~EventLoopThreadPool()
{
    //停止线程池
    _threadpool.stop();
}

void EventLoopThreadPool::startPool(int numsEventLoops)
{
    //创建一个线程池
    _threadpool.start(numsEventLoops);
    //每个线程执行一个EventLoop的loop函数
    //Task 绑定为EventLoop的loop 函数
    for(int i = 0; i < numsEventLoops; i++)
    {
        EventLoop *loop = new EventLoop();
        _eventloops.push_back(loop);
        _threadpool.run(std::bind(&EventLoop::loop, loop));
    }
}


EventLoop* EventLoopThreadPool::getNextLoop()
{
    //只有一个线程维护该线程池，所以分配不需要考虑线程安全问题
    EventLoop *loop = _baseloop;
    if(!_eventloops.empty())
    {
        loop = _eventloops[_next];
        ++_next;
        if(_next == _eventloops.size())
        {
            _next = 0;
        }
    }
    return loop;
}

//通过hash 的方式获得loop 
//实现一个hash函数
EventLoop * EventLoopThreadPool::getHashLoop()
{
    //暂时不实现该函数
}

EventLoopThreadPool::EventloopList EventLoopThreadPool::getAllLoop()
{
    if(_eventloops.empty())
    {
        return std::vector<EventLoop*>(1, _baseloop);    
    }
    else
    {
        return _eventloops;
    }
}
