#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <functional>
#include <stdexcept>
#include <utility>

namespace Plazza {

    /**
     * @class Process
     * @brief Encapsulation of a system process.
     * 
     * Manages the lifecycle of a child process, providing safe fork and wait mechanisms.
     */
    class Process {
    public:
        /**
         * @brief Construct a new Process object.
         */
        Process() = default;

        /**
         * @brief Destroy the Process object.
         * 
         * Automatically waits for the child process if it's still running,
         * avoiding zombie processes.
         */
        ~Process();

        /* Disable copying */
        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;

        /* Allow moving */
        Process(Process&& other) noexcept;
        Process& operator=(Process&& other) noexcept;

        /**
         * @brief Start a new process.
         * 
         * Forks the current process. The provided callable is executed in the child process.
         * 
         * @tparam Function The type of the callable to execute in the child.
         * @tparam Args The types of the arguments to pass.
         * @param f The callable to execute.
         * @param args The arguments.
         */
        template <typename Function, typename... Args>
        void start(Function&& f, Args&&... args) {
            if (_pid > 0) {
                wait(); // Wait for existing process before starting a new one
            }

            _pid = fork();
            if (_pid < 0) {
                throw std::runtime_error("Failed to fork process");
            }

            if (_pid == 0) {
                // In child process
                f(std::forward<Args>(args)...);
                std::exit(0);
            }
            // In parent process, _pid is the child's PID.
        }

        /**
         * @brief Block the current process until this child process finishes.
         */
        void wait();

        /**
         * @brief Check if the child process is still running.
         * 
         * @return true If the child is executing.
         * @return false Otherwise.
         */
        bool isAlive() const;

        /**
         * @brief Get the PID of the process.
         * 
         * @return pid_t The process ID (-1 if no process is currently managed).
         */
        pid_t getPid() const;

    private:
        pid_t _pid = -1; /**< The process ID, -1 if no process is managed */
    };
}
