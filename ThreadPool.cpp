#include "ThreadPool.h"
#include <iostream>

using namespace std;

//这里要求传入的任务队列的大小一定要大于0
ThreadPool::ThreadPool(int size)
    :_maxQueueTaskSize(size),
    running(false)
{
    if(_maxQueueTaskSize <= 0)
    {
        /*
         * 这里可以设置日志文件记录程序结束的原因
         */
        exit(1);
    }
    //初始化线程锁于条件变量
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_condThread, NULL);
    pthread_cond_init(&_condQueue, NULL);
}

static void *staticRunInThread(void *arg)
{
    ThreadPool*p = (ThreadPool*)arg;
    p->runInThread();
}

//开启线程池
void ThreadPool::start(int numsThread)
{
    running = true;
    if(numsThread >= 0)
    {
        _threads.resize(numsThread);
    }
    else
    {
        return ;
    }
    for(int i = 0; i < numsThread; i++)
    {
        //绑定子线程
        pthread_create(&_threads[i], NULL, staticRunInThread, (void*)this);
    }
    //修改线程池的运行状态
}

//关闭一个线程池
void ThreadPool::stop()
{
    if(running)
    {
        running = false;
        //唤醒所有的线程，结束执行
        pthread_cond_broadcast(&_condThread);
        int size = _threads.size();
        for(int i = 0; i < size; i++)
        {
            pthread_join(_threads[i], NULL);
        }
    }
}

//因为判断之后往往伴随着增上的操作，在进行判断之前要进行加锁，而不是
//在函数内部进行加锁，减少上锁的次数
bool ThreadPool::isFull()
{
    return _queueTask.size() >= _maxQueueTaskSize;    
}

//将一个要执行的任务放入任务队列中运行
void ThreadPool::run(Task task)
{
    //如果线程池中没有线程，主线程执行
    if(_threads.empty())
    {
        task();
    }
    else
    {
        pthread_mutex_lock(&_mutex);
        if(isFull())
        {
            pthread_cond_wait(&_condQueue, &_mutex);
        }
        //断言任务队列不满
        assert(!isFull());

        cout << "将任务加入到队列" << endl;
        _queueTask.push(std::move(task));
        //
        pthread_mutex_unlock(&_mutex);
        cout << "队列中添加一个任务，就唤醒一个线程去执行" << endl;
        pthread_cond_signal(&_condThread);
    }
}

//
void ThreadPool::runInThread()
{
    //线程池中的线程循环获取获取任务队列的任务
    while(running)
    {
        Task func = take();
        if(func)
        {
            cout << "开始执行函数" << endl;
            func();
        }
    }
}

//线程池中的线程获取一个任务
ThreadPool::Task ThreadPool::take()
{
    Task task = NULL;
    pthread_mutex_lock(&_mutex);
    while(_queueTask.empty() && running)
    {
        //没有任务的时候线程会在此睡眠
        pthread_cond_wait(&_condThread, &_mutex);
    }
    if(!_queueTask.empty())
    {
        task = _queueTask.front();
        _queueTask.pop();
    }
    //处理一个任务后任务队列一定不满，唤醒可以添加任务的线程
    pthread_cond_signal(&_condQueue);

    pthread_mutex_unlock(&_mutex);

    return task;
}

/*
#include <iostream>

using namespace std;


void fun()
{
    cout << "123" << endl;
}


int main()
{
    ThreadPool _threadpool;
    _threadpool.start(10);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);
    _threadpool.run(fun);

    sleep(1);
}
*/
