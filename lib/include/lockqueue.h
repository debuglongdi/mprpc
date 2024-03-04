#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace mprpc
{
template<typename T>
class LockQueue
{
public:
    //多个线程向缓冲队列中写入日志
    void push(T &data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(data);
        cv_.notify_one();
    }

    //只有一个线程从队列中取出数据， 再写入日志文件
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(queue_.empty())
        {
            //日志队列为空
            cv_.wait(lock);
        }
        T data = queue_.front();
        queue_.pop();
        return data;
    }
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
} // namespace mprpc
