#include "Poller.h"

#include <vector>
#include <poll.h>

using namespace std;

class PollPoller:public Poller
{
public:
    PollPoller(EventLoop *loop):Poller(loop)
    {}

    void poll(ChannelList &_activeChannel);

    void updateChannel(Channel *channel);

    void removeChannel(Channel *channel);

    void fillActiveChannels(int nums, ChannelList &_activeChannel);
private:
    typedef vector<struct pollfd> PollfdList;
    PollfdList _pollfds;
};
