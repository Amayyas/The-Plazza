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
    template <typename T>
    class SafeQueue {
        public:
            SafeQueue() : _isOpen(true) {}
            ~SafeQueue() = default;

            void push(const T& value) {
                _mutex.lock();

                if (_isOpen) {
                    _queue.push(value);
                    _cv.notifyOne();
                }

                _mutex.unlock();
            }

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

            void close() {
                _mutex.lock();
                _isOpen = false;
                _cv.notifyAll();
                _mutex.unlock();
            }

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
