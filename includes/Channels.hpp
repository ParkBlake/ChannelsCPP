#ifndef CHANNELS_H
#define CHANNELS_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class Channel {
public:
    void send(const T& value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
        }
        cv_.notify_one();
    }

    std::shared_ptr<T> receive() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty(); });
        if (queue_.empty()) {
            return nullptr;
        }
        auto value = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return value;
    }

    std::shared_ptr<T> try_receive() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        auto value = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return value;
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // CHANNELS_H
