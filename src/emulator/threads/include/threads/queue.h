#ifndef queue_h
#define queue_h

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Queue {
public:
    unsigned int displayQueueMaxPendingCount_;

    T pop() {
        T item;
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (!aborted && queue_.empty()) {
                condempty_.wait(mlock);
            }

            if (aborted) {
                mlock.release();
                return item;
            }
            item = queue_.front();
            queue_.pop();
        }
        cond_.notify_one();
        return item;
    }

    void push(const T &item) {
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (!aborted && queue_.size() == displayQueueMaxPendingCount_) {
                cond_.wait(mlock);
            }
            if (aborted) {
                mlock.release();
                return;
            }
            queue_.push(item);
        }
        condempty_.notify_one();
    }

    void abort() {
        aborted = true;
        condempty_.notify_all();
        cond_.notify_all();
    }

    void reset() {
        queue_.clear();
        aborted = false;
    }

    Queue() = default;
    Queue(const Queue &) = delete; // disable copying
    Queue &operator=(const Queue &) = delete; // disable assignment

private:
    std::condition_variable cond_;
    std::condition_variable condempty_;
    std::queue<T> queue_;
    std::mutex mutex_;
    bool aborted = false;
};

#endif /* queue_h */
