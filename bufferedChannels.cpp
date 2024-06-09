#include <iostream>
#include <thread>
#include <chrono>
#include "bufferedChannels.hpp"

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
        } else {

            std::cout << "Consumer " << id << " has nothing to receive" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MESSAGE_DELAY_CONSUMER_MS));

    }
}

int main() {
    BufferedChannel<int> ch(5);

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

    auto valueWithTimeout = ch.receive_with_timeout(std::chrono::milliseconds(500));

    if (valueWithTimeout.has_value()) {

        std::cout << "Received with timeout: " << *valueWithTimeout << std::endl;

    } else {

        std::cout << "Timeout occurred, nothing received" << std::endl;

        ch.clear();
        std::cout << "Channel cleared, size: " << ch.size() << std::endl;

        auto nextElement = ch.peek();

        if (nextElement.has_value()) {

            std::cout << "Next element in channel: " << *nextElement << std::endl;

        } else {

            std::cout << "Channel is empty" << std::endl;

        }

        std::cout << "Channel size: " << ch.size() << std::endl;

    }
}

