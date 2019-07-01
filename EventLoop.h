#pragma once
#include <vector>
#include <functional>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sys/eventfd.h>

class Channel;
class Poller;
typedef std::vector<Channel *> ChannelList;
class EventLoop
{
public:
    typedef std::function<void()> Functor;

    EventLoop();

    void loop();

    void quit();

    void weakup();

    int createEventfd();

    //用户唤醒线程的读操作
    void handleRead();

    bool isInLoopThread(){ return _threadId == pthread_self(); }

    void doFunctors();

    void runInLoop(Functor cb);

    void queueInLoop(Functor cb);

    void updateChannel(Channel *channel);

    void removeChannel(Channel *channel);
private:
    ChannelList _activeChannel;
    Poller *_poller;

    //当前EventLoop所在线程地id
    pthread_t _threadId;
    //存放需要处理地事件
    std::vector<Functor> _functors;
    
    //用于唤醒EventLoop
    int _weakupFd;

    Channel *_channelWeakup;
    pthread_mutex_t _mutex;
    bool _quit;
};
