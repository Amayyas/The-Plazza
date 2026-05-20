/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Mutex.cpp
*/

#include "Mutex.hpp"

namespace Plazza {
    void Mutex::lock() {
        _mutex.lock();
    }

    void Mutex::unlock() {
        _mutex.unlock();
    }

    bool Mutex::try_lock() {
        return _mutex.try_lock();
    }

    std::mutex& Mutex::getNative() {
        return _mutex;
    }
}
