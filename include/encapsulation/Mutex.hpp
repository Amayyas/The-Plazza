/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Mutex.hpp
*/

#pragma once

#include <mutex>

namespace Plazza {
    /**
     * @class Mutex
     * @brief Encapsulation of a standard C++ mutex.
     * 
     * Provides a thread-safe locking mechanism to protect shared data
     * from being simultaneously accessed by multiple threads.
     */
    class Mutex {
    public:
        /**
         * @brief Construct a new Mutex object.
         */
        Mutex() = default;

        /**
         * @brief Destroy the Mutex object.
         */
        ~Mutex() = default;

        /**
         * @brief Lock the mutex.
         * 
         * Blocks the current thread until the mutex can be locked.
         */
        void lock();

        /**
         * @brief Unlock the mutex.
         * 
         * Releases the lock held by the current thread.
         */
        void unlock();

        /**
         * @brief Try to lock the mutex without blocking.
         * 
         * @return true If the lock was acquired successfully.
         * @return false If the lock is already held by another thread.
         */
        bool try_lock();

        /**
         * @brief Get the native std::mutex object.
         * 
         * Useful for compatibility with standard C++ synchronization primitives
         * like std::unique_lock.
         * 
         * @return std::mutex& Reference to the underlying std::mutex.
         */
        std::mutex& getNative();

    private:
        std::mutex _mutex; /**< The underlying standard mutex */
    };
}
