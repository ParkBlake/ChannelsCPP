# Concurrent C++ Channel Library

A simple and lightweight C++ library for creating channels, facilitating communication between threads.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Prerequisites](#prerequisites)
- [Building from Source](#building-from-source)
- [Usage](#usage)
- [Unbuffered Channels](#unbuffered-channels)
- [Buffered Channels](#buffered-channels)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Concurrent C++ Channel Library provides a convenient way to manage communication between threads in C++ programs. It offers support for both unbuffered and buffered channels, ensuring thread safety and compatibility with major operating systems and C++ compilers.

## Features

- **Unbuffered Channels**: Support for unbuffered channels for synchronous message passing.
- **Buffered Channels**: Optionally support buffered channels for asynchronous message passing with blocking and non-blocking operations.
- **Thread-Safe**: Ensure thread safety for concurrent access from multiple threads.
- **Cross-Platform**: Compatible with major operating systems and C++ compilers.

## Installation

### Prerequisites

Before using the library, ensure you have the following installed:

- C++17 or later
- A C++ compiler (e.g., GCC, Clang, MSVC)

### Building from Source

Clone the repository:

git clone https://github.com/ParkBlake/cpp-channel-library.git

### Usage

- Unbuffered Channels

channels.hpp

The channels.hpp header provides support for unbuffered channels, enabling synchronous message passing between threads.

Usage example:

C++

#include "channels.hpp"

int main() {

    Channel<int> ch;

    // Producer thread
    std::thread producer([&ch]() {
        ch.send(42);
    });

    // Consumer thread
    std::thread consumer([&ch]() {
        auto value = ch.receive();
        std::cout << "Received: " << *value << std::endl;
    });

    producer.join();
    consumer.join();

}

- Buffered Channels

bufferedChannels.hpp

The bufferedChannels.hpp header provides support for buffered channels, allowing asynchronous message passing with blocking and non-blocking operations.

### Usage example:

C++

#include "bufferedChannels.hpp"

int main() {

    BufferedChannel<int> ch(3);

    // Producer thread
    std::thread producer([&ch]() {
        ch.send(42);
    });

    // Consumer thread
    std::thread consumer([&ch]() {
        auto value = ch.receive();
        std::cout << "Received: " << *value << std::endl;
    });

    producer.join();
    consumer.join();

}

### Contributing

Contributions are welcome! Fork the repository, make your changes, and submit a pull request.
License

This project is licensed under the MIT License. See the LICENSE file for details.