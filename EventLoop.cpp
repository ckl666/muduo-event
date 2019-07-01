#include "EventLoop.h"
#include "Channel.h"
#include "PollPoller.h"

#include <iostream>

using namespace std;

//构造函数
EventLoop::EventLoop()
        :_poller(new PollPoller(this)),
         _quit(false),
         //获取当前线程地id
         _threadId(pthread_self()),
         _weakupFd(createEventfd()),
         _channelWeakup(new Channel(this, _weakupFd))
{
    //初始化锁
    pthread_mutex_init(&_mutex, NULL);
    _channelWeakup->setReadCallBack(std::bind(&EventLoop::handleRead,this));
    _channelWeakup->enableReading();
}

int EventLoop::createEventfd()
{
    int evfd = eventfd(0,EFD_NONBLOCK);
    assert(evfd != -1);
    return evfd;
}

void EventLoop::loop()
{
    if(!isInLoopThread())
    {
        //设置当前EventLoop所在线程的id 
        cout << "设置新的线程Id" << endl;
        _threadId = pthread_self();
    }
    _quit = false;
    while(!_quit)
    {
        //清除原来的就绪事件
        _activeChannel.clear();
        _poller->poll(_activeChannel);
        for(Channel *channel:_activeChannel)
        {
            channel->handleEvent();
        }
        //执行functors中地任务
        doFunctors();
    }
}


void EventLoop::runInLoop(Functor cb)
{
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        //cb();
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    pthread_mutex_lock(&_mutex);      
    _functors.push_back(std::move(cb));
    pthread_mutex_unlock(&_mutex);        
    if(!isInLoopThread())
    {
        cout << "唤醒一个EventLoop" << endl;
        //唤醒EventLoop
        weakup();
    }
}


void EventLoop::doFunctors()
{
    pthread_mutex_lock(&_mutex);
    while(!_functors.empty())
    {
        Functor cb = _functors.back();
        _functors.pop_back();
        cout << "调用回掉函数read" << endl;
        cb();
    }
    pthread_mutex_unlock(&_mutex);
}


void EventLoop::quit()
{
    _quit = true;
    if(!isInLoopThread())
    {
        weakup();
    }
}

void EventLoop::weakup()
{
    uint64_t count;
    int res = write(_weakupFd, &count, sizeof(count));
    assert(res >= 0);
}

void EventLoop::handleRead()
{
    uint64_t count;
    int res = read(_weakupFd, &count, sizeof(count));
    assert(res >= 0);
}


void EventLoop::updateChannel(Channel *channel)
{
    assert(channel != NULL);
    //runInLoop()
    _poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel != NULL);
    _poller->removeChannel(channel);
}
