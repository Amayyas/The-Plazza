/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Thread.hpp
*/

#pragma once

#include <thread>
#include <functional>
#include <utility>

namespace Plazza {
    /**
     * @class Thread
     * @brief Encapsulation of a standard C++ execution thread.
     * 
     * Manages the lifecycle of a thread, providing safe start, join, and detach mechanisms.
     */
    class Thread {
    public:
        /**
         * @brief Construct a new Thread object (empty state).
         */
        Thread() = default;

        /**
         * @brief Destroy the Thread object.
         * 
         * Automatically joins the thread if it is still joinable to prevent std::terminate.
         */
        ~Thread();

        /* Disable copying: thread wrappers are not copyable. */
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        /* Allow moving: propagate the move semantics of std::thread. */
        Thread(Thread&&) noexcept = default;
        Thread& operator=(Thread&&) noexcept = default;

        /**
         * @brief Start a new execution thread.
         * 
         * @tparam Function The type of the callable to execute.
         * @tparam Args The types of the arguments to pass to the callable.
         * @param f The callable to execute in the new thread.
         * @param args The arguments to pass to the callable.
         */
        template <typename Function, typename... Args>
        void start(Function&& f, Args&&... args) {
            if (_thread.joinable()) {
                // Joining existing thread prevents calling std::thread::operator=(thread)
                // on a joinable thread which would call std::terminate.
                _thread.join();
            }
            _thread = std::thread(std::forward<Function>(f), std::forward<Args>(args)...);
        }

        /**
         * @brief Block the current thread until this thread finishes execution.
         */
        void join();

        /**
         * @brief Separate the thread of execution from the thread object.
         * 
         * Allows execution to continue independently.
         */
        void detach();

        /**
         * @brief Check if the thread is joinable.
         * 
         * @return true If the thread is executing and can be joined.
         * @return false Otherwise.
         */
        bool joinable() const;

        /**
         * @brief Sleeps for a specified duration, but wakes up immediately if the boolean becomes false.
         * @param durationMs The total waiting time in milliseconds.
         * @param running Optional pointer to the activity boolean.
         */
        static void sleep(std::size_t durationMs, const bool *running = nullptr);

    private:
        std::thread _thread; /**< The underlying execution thread */
    };
}
