#ifndef BUFFERED_CHANNELS_H
#define BUFFERED_CHANNELS_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <chrono>
#include <future>

template<typename T>
class BufferedChannel {

public:

    explicit BufferedChannel(size_t capacity) : capacity_(capacity) {}

    void send(T value) {

        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_not_full_.wait(lock, [this] { return queue_.size() < capacity_; });

            queue_.push(std::move(value));
        }
        cv_not_empty_.notify_one();
    }

    std::optional<T> receive() {

        std::unique_lock<std::mutex> lock(mutex_);
        cv_not_empty_.wait(lock, [this] { return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        cv_not_full_.notify_one();
        return value;

    }

    std::optional<T> receive_with_timeout(std::chrono::milliseconds timeout) {

        std::unique_lock<std::mutex> lock(mutex_);
        if (cv_not_empty_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {

            T value = std::move(queue_.front());
            queue_.pop();
            cv_not_full_.notify_one();
            return value;

        } else {

            return std::nullopt;

        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!queue_.empty()) {

            queue_.pop();
        }
    }

    std::optional<T> peek() const {

        std::lock_guard<std::mutex> lock(mutex_);
        if (!queue_.empty()) {

            return queue_.front();

        } else {

            return std::nullopt;
        }
    }

    size_t size() const {

        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();

    }

private:

    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_not_empty_;
    std::condition_variable cv_not_full_;
    size_t capacity_;

};

#endif // BUFFERED_CHANNELS_H
