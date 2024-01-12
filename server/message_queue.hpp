#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class MessageQueue
{
private:
    std::queue<T> _queue;
    std::mutex _mtx;
    std::condition_variable _cv;

public:
    void push(const T &msg)
    {
        std::unique_lock<std::mutex> lck(_mtx);
        _queue.push(msg);
        _cv.notify_one();
    }

    bool wait(T &msg, std::int32_t ms_out_time = 0)
    {
        std::unique_lock<std::mutex> lck(_mtx);

        if (ms_out_time <= 0)
        {
            _cv.wait(lck, [this]
                     { return _queue.size() > 0; });
        }
        else
        {
            _cv.wait_for(lck, std::chrono::milliseconds(ms_out_time), [this]
                         { return _queue.size() > 0; });
        }

        if (_queue.size() > 0)
        {
            msg = _queue.front();
            _queue.pop();

            return true;
        }
        return false;
    }

    bool poll(T &msg)
    {
        std::unique_lock<std::mutex> lck(_mtx);
        if (_queue.size() > 0)
        {
            msg = _queue.front();
            _queue.pop();

            return true;
        }

        return false;
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lck(_mtx);
        return _queue.size();
    }
};

#endif
