#pragma once

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <functional>
#include <queue>
#include <assert.h>

class ThreadPool
{
public:
    typedef std::function<void(void)> Task;

    ThreadPool(int size = 10);
    
    void start(int numsThread);

    void run(Task task);
    
    void stop();

    bool isFull();

    void runInThread();

    Task take();
private:
    std::queue<Task> _queueTask;
    int _maxQueueTaskSize;

    std::vector<pthread_t> _threads;

    //线程池的状态，是否在运行态
    bool running;

    //控制线程队列和任务队列的安全访问
    pthread_mutex_t _mutex;

    //两个条件变量，一个用来同步添加任务的线程，一个用来同步线程池中的线程
    pthread_cond_t _condThread;
    pthread_cond_t _condQueue;
};
