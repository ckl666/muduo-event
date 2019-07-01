#include "PollPoller.h"

#include <iostream>

using namespace std;

//
void PollPoller::poll(ChannelList &_activeChannel)
{
    int nums = ::poll(&*_pollfds.begin(), _pollfds.size(), 5000);
    if(nums > 0)
    {
        fillActiveChannels(nums,_activeChannel);
    }
    else if(nums == 0)
    {
        //timeout
    }
    else
    {
        //error
    }
}
void PollPoller::fillActiveChannels(int nums, ChannelList &_activeChannel)
{
    for(auto pfd:_pollfds)
    {
        if(pfd.revents > 0)
        {
            --nums;
            Channel *channel = _channels[pfd.fd];
            channel->setRevents(pfd.revents);
            _activeChannel.push_back(channel);
        }
        if(nums <= 0)
        {
            return ;
        }
    }
}

void PollPoller::updateChannel(Channel *channel)
{
    cout << "添加一个文件描述符" << endl;
    //如果是添加一个新的文件描述符
    int idx = channel->getIndex();
    if(idx < 0)
    {
        struct pollfd pfd;
        pfd.fd = channel->getFd();
        pfd.events = channel->getEvents();
        pfd.revents = 0;
        _pollfds.push_back(pfd);
        idx = _pollfds.size()-1;
        channel->setIndex(idx);
        _channels[pfd.fd] = channel;
    }
    //更新现有的文件描述符上的事件
    else
    {
        struct pollfd &pfd = _pollfds[idx];
        pfd.fd = channel->getFd();
        pfd.events = channel->getEvents();
        pfd.revents = 0;
    }
}

void PollPoller::removeChannel(Channel *channel)
{
    int idx = channel->getIndex();
    if(idx < 0)
    {
        //不存在
        return ;
    }
    //关闭套接字
    close(channel->getFd());
    //_pollfds.swap(_pollfds.begin()+idx, _pollfds.end()-1);
    _pollfds[idx] = _pollfds[_pollfds.size()-1];
    int fd = _pollfds[idx].fd;
    _channels[fd]->setIndex(idx);
    _pollfds.pop_back();
    _channels.erase(channel->getFd());
}
