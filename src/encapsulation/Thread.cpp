/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Thread.cpp
*/

#include "Thread.hpp"
#include <stdexcept>
#include <thread>

namespace Plazza {
    Thread::~Thread() {
        if (_thread.joinable()) {
            if (_thread.get_id() == std::this_thread::get_id()) {
                _thread.detach();
            } else {
                _thread.join();
            }
        }
    }

    void Thread::join() {
        if (_thread.joinable()) {
            if (_thread.get_id() == std::this_thread::get_id()) {
                throw std::logic_error("Thread::join() called on the current thread");
            }
            _thread.join();
        }
    }

    void Thread::detach() {
        if (_thread.joinable()) {
            _thread.detach();
        }
    }

    bool Thread::joinable() const {
        return _thread.joinable();
    }
}
