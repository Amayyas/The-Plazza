#pragma once

#include <string>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace Plazza {

    /**
     * @class IPC
     * @brief Inter-Process Communication using a bidirectional channel.
     * 
     * Encapsulates a socketpair to provide bidirectional message passing
     * between a parent and a child process. Handles packet boundaries
     * to ensure safe delivery of messages via operator overloading.
     */
    class IPC {
    public:
        /**
         * @brief Construct a new IPC object.
         * 
         * Creates a bidirectional communication channel (socketpair).
         * Must be instantiated before forking.
         */
        IPC();

        /**
         * @brief Destroy the IPC object.
         * 
         * Closes any open file descriptors.
         */
        ~IPC();

        /* Disable copying */
        IPC(const IPC&) = delete;
        IPC& operator=(const IPC&) = delete;

        /* Allow moving */
        IPC(IPC&& other) noexcept;
        IPC& operator=(IPC&& other) noexcept;

        /**
         * @brief Configure the IPC for the parent process.
         * 
         * Closes the child's side of the channel and sets the active FD.
         * Should be called in the parent immediately after fork.
         */
        void setParentMode();

        /**
         * @brief Configure the IPC for the child process.
         * 
         * Closes the parent's side of the channel and sets the active FD.
         * Should be called in the child immediately after fork.
         */
        void setChildMode();

        /**
         * @brief Check if data is available to read without blocking.
         * @param timeoutMs Max time to wait in milliseconds.
         * @return true if data is ready, false if timeout reached.
         */
        bool hasData(int timeoutMs);

        /**
         * @brief Send a message through the IPC channel.
         * 
         * Prepends the message size to ensure boundary preservation.
         * 
         * @param message The message to send.
         * @return IPC& Reference to this object for chaining.
         */
        IPC& operator<<(const std::string& message);

        /**
         * @brief Receive a message from the IPC channel.
         * 
         * Blocks until a complete packet is received.
         * 
         * @param message The string to populate with the received message.
         * @return IPC& Reference to this object for chaining.
         */
        IPC& operator>>(std::string& message);

    private:
        int _socketPair[2]; /**< The socketpair file descriptors */
        int _activeFd;      /**< The active file descriptor for the current mode */
    };
}
