#include <iostream>
#include <thread>
#include "bufferedChannels.hpp" // Correct header file name

constexpr int NUM_PRODUCERS = 3;
constexpr int NUM_CONSUMERS = 2;
constexpr int MESSAGE_DELAY_PRODUCER_MS = 100;
constexpr int MESSAGE_DELAY_CONSUMER_MS = 150;

void producerFunc(BufferedChannel<int>& ch, int id) {
    for (int i = 0; i < 5; ++i) {
        ch.send(id * 10 + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(MESSAGE_DELAY_PRODUCER_MS));
        std::cout << "Producer " << id << " sent: " << id * 10 + i << std::endl;
    }
}

void consumerFunc(BufferedChannel<int>& ch, int id) {
    for (int i = 0; i < 5; ++i) {
        auto value = ch.receive();
        if (value.has_value()) {
            std::cout << "Consumer " << id << " received: " << *value << std::endl;
        }
        else {
            std::cout << "Consumer " << id << " has nothing to receive" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MESSAGE_DELAY_CONSUMER_MS));
    }
}

/* int main() {
    BufferedChannel<int> ch(5); // BufferedChannel with capacity 5

    std::vector<std::thread> producerThreads;
    producerThreads.reserve(NUM_PRODUCERS);
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producerThreads.emplace_back(producerFunc, std::ref(ch), i);
    }

    std::vector<std::thread> consumerThreads;
    consumerThreads.reserve(NUM_CONSUMERS);
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumerThreads.emplace_back(consumerFunc, std::ref(ch), i);
    }

    for (auto& thread : producerThreads) {
        thread.join();
    }

    for (auto& thread : consumerThreads) {
        thread.join();
    }
} */