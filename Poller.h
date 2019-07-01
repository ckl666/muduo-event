#pragma once

#include <unistd.h>
#include "EventLoop.h"
#include "Channel.h"
#include <unordered_map>

using namespace std;

class Poller
{
public:
    Poller(EventLoop *loop):_loop(loop)
    {}

    ~Poller();
    
    virtual void poll(ChannelList &_activeChannel) = 0;

    virtual void updateChannel(Channel *channel) = 0;

    virtual void removeChannel(Channel *channel) = 0;

protected:
    typedef std::unordered_map<int,Channel*> ChannelMap;
    ChannelMap _channels;
private:
    EventLoop *_loop;
};
