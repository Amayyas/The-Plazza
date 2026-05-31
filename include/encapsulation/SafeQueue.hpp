/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** SafeQueue.hpp
*/

/// @file SafeQueue.hpp
/// @brief SafeQueue class definition

#pragma once

#include <queue>
#include "Mutex.hpp"
#include "ConditionVariable.hpp"

namespace Plazza {
    ///
    /// @class SafeQueue
    /// @brief Thread-safe blocking queue used as the pizza work queue inside each Kitchen.
    ///
    /// Producers call push() to enqueue items; consumers call pop(), which blocks until
    /// an item is available or the queue is closed. Closing the queue (close()) unblocks
    /// all waiting consumers so threads can exit cleanly.
    ///
    template <typename T>
    class SafeQueue {
        public:
            /// @brief Construct an open queue.
            SafeQueue() : _isOpen(true) {}
            ~SafeQueue() = default;

            /// @brief Push an item onto the queue and wake one waiting consumer.
            /// @param value The item to enqueue (ignored if the queue is closed).
            void push(const T& value) {
                _mutex.lock();

                if (_isOpen) {
                    _queue.push(value);
                    _cv.notifyOne();
                }

                _mutex.unlock();
            }

            /// @brief Pop an item, blocking until one is available or the queue closes.
            /// @param value Output parameter filled with the dequeued item.
            /// @return true on success, false if the queue is closed and empty.
            bool pop(T& value) {
                _mutex.lock();

                while (_queue.empty() && _isOpen) {
                    _cv.wait(_mutex);
                }

                if (!_isOpen && _queue.empty()) {
                    _mutex.unlock();
                    return false;
                }

                value = _queue.front();
                _queue.pop();
                _mutex.unlock();
                return true;
            }

            /// @brief Return a snapshot of all queued items without removing them.
            /// @return std::vector<T> Copy of the current queue contents.
            std::vector<T> getItems() {
                _mutex.lock();
                std::queue<T> copy = _queue;
                _mutex.unlock();

                std::vector<T> items;
                while (!copy.empty()) {
                    items.push_back(copy.front());
                    copy.pop();
                }
                return items;
            }

            /// @brief Close the queue and wake all blocked consumers so they can exit.
            void close() {
                _mutex.lock();
                _isOpen = false;
                _cv.notifyAll();
                _mutex.unlock();
            }

            /// @brief Return the current number of items in the queue.
            size_t size() {
                _mutex.lock();
                size_t s = _queue.size();
                _mutex.unlock();
                return s;
            }

        private:
            std::queue<T> _queue;
            Plazza::Mutex _mutex;
            Plazza::ConditionVariable _cv;
            bool _isOpen;
    };
} // namespace Plazza
