/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** ConditionVariable.hpp
*/

#pragma once

#include "Mutex.hpp"
#include <condition_variable>

namespace Plazza {
    /**
     * @class ConditionVariable
     * @brief Encapsulation of a standard C++ condition variable.
     * 
     * Allows threads to wait until notified by another thread that a shared 
     * state has changed.
     */
    class ConditionVariable {
    public:
        /**
         * @brief Construct a new Condition Variable object.
         */
        ConditionVariable() = default;

        /**
         * @brief Destroy the Condition Variable object.
         */
        ~ConditionVariable() = default;

        /**
         * @brief Wait for the condition variable to be notified.
         * 
         * Atomically unlocks the mutex and blocks the current thread.
         * When notified, the thread is unblocked and the mutex is locked again.
         * 
         * @param mutex The mutex to unlock and re-lock. Must be locked by the current thread before calling.
         */
        void wait(Mutex& mutex);

        /**
         * @brief Notify a single waiting thread.
         * 
         * If any threads are waiting on this condition variable, unblocks one of them.
         */
        void notifyOne();

        /**
         * @brief Notify all waiting threads.
         * 
         * Unblocks all threads currently waiting on this condition variable.
         */
        void notifyAll();

    private:
        std::condition_variable _cv; /**< The underlying condition variable */
    };
}
