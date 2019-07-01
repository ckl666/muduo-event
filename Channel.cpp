#include "Channel.h"


int Channel::ReadEvent = POLLIN;
int Channel::WriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    :_fd(fd),
    _loop(loop),
    _index(-1),
    _events(0),
    _revents(0),
    writeCallback(NULL),
    readCallback(NULL)
{}

void Channel::update()
{
    _loop->updateChannel(this);
}

void Channel::removeChannel()
{
    _loop->removeChannel(this);
    //_loop->runInLoop(std::bind(_loop->removeChannel, _loop,this));
}

void Channel::enableReading()
{
    _events |= ReadEvent;
    update();
}

void Channel::cancleReading()
{
    _events &= ~ReadEvent;
    update();
}

void Channel::enableWritting()
{
    _events |= ReadEvent;
    update();
}

void Channel::cancleWritting()
{
    _events &= ~WriteEvent;
    update();
}

void Channel::cancleAllEvent()
{
    _events = 0;
    update();
}

//事件分发，调用回调函数
void Channel::handleEvent()
{
    //处理读事件
    if(_revents & ReadEvent)
    {
        if(readCallback)
        {
            readCallback();
        }
    }
    //处理写事件
    if(_revents & WriteEvent)
    {
        if(writeCallback)
        {
            writeCallback();
        }
    }
}

