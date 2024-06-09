#ifndef CHANNELS_H
#define CHANNELS_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>
#include <atomic>

template<typename T>
class Channel {
public:
    explicit Channel(size_t max_capacity = std::numeric_limits<size_t>::max()) : max_capacity_(max_capacity) {}

    void send(const T& value) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_not_full_.wait(lock, [this] { return queue_.size() < max_capacity_; });
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
        cv_not_full_.notify_one();
        return value;
    }

    std::shared_ptr<T> try_receive() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        auto value = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        cv_not_full_.notify_one();
        return value;
    }

    std::shared_ptr<T> receive_with_timeout(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (cv_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            auto value = std::make_shared<T>(std::move(queue_.front()));
            queue_.pop();
            cv_not_full_.notify_one();
            return value;
        }
        return nullptr; // Timeout occurred
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!queue_.empty()) {
            queue_.pop();
        }
        cv_not_full_.notify_all();
    }

    std::shared_ptr<T> peek() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        return std::make_shared<T>(queue_.front());
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::condition_variable cv_not_full_;
    size_t max_capacity_;
};

#endif // CHANNELS_H
