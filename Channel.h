#pragma once
#include "EventLoop.h"
#include <poll.h>
#include <functional>

typedef std::function<void(void)> EventCallback;
class Channel
{
public:
    Channel(EventLoop *loop, int sockfd);

    void update();

    void removeChannel();
    
    void handleEvent();

    void enableReading();

    void cancleReading();

    void cancleAllEvent();

    //设置写的回调函数
    void setWriteCallBack(EventCallback cb)
    {
        writeCallback = cb;
    }

    //设置读的回调函数
    void setReadCallBack(EventCallback cb)
    {
        readCallback = cb;
    }

    void enableWritting();

    void cancleWritting();
    
    void setIndex(int idx) { _index = idx; }

    int getFd(){ return _fd; }

    int getEvents() { return _events; }

    int getRevents() { return _revents; }

    int getIndex() { return _index; }

    void setFd(int fd) { _fd = fd; }

    void setEvents(int events) { _events = events; }

    void setRevents(int revents) { _revents = revents; }
private:
    EventLoop *_loop;

    int _fd;
    int _events;
    int _revents;
    int _index;

    EventCallback writeCallback;
    EventCallback readCallback;

    static int ReadEvent;
    static int WriteEvent;
};
