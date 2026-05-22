#include "../../include/IPC/IPC.hpp"
#include <unistd.h>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <cerrno>

namespace Plazza {

    IPC::IPC() {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, _socketPair) == -1) {
            throw std::runtime_error("Failed to create socketpair for IPC");
        }
        _activeFd = -1;
    }

    IPC::~IPC() {
        if (_socketPair[0] != -1) {
            close(_socketPair[0]);
        }
        if (_socketPair[1] != -1) {
            close(_socketPair[1]);
        }
    }

    IPC::IPC(IPC&& other) noexcept : _activeFd(other._activeFd) {
        _socketPair[0] = other._socketPair[0];
        _socketPair[1] = other._socketPair[1];
        other._socketPair[0] = -1;
        other._socketPair[1] = -1;
        other._activeFd = -1;
    }

    IPC& IPC::operator=(IPC&& other) noexcept {
        if (this != &other) {
            if (_socketPair[0] != -1) close(_socketPair[0]);
            if (_socketPair[1] != -1) close(_socketPair[1]);

            _socketPair[0] = other._socketPair[0];
            _socketPair[1] = other._socketPair[1];
            _activeFd = other._activeFd;

            other._socketPair[0] = -1;
            other._socketPair[1] = -1;
            other._activeFd = -1;
        }
        return *this;
    }

    void IPC::setParentMode() {
        if (_socketPair[1] != -1) {
            close(_socketPair[1]);
            _socketPair[1] = -1;
        }
        _activeFd = _socketPair[0];
    }

    void IPC::setChildMode() {
        if (_socketPair[0] != -1) {
            close(_socketPair[0]);
            _socketPair[0] = -1;
        }
        _activeFd = _socketPair[1];
    }

    bool IPC::hasData(int timeoutMs)
    {
        struct pollfd pfd;

        pfd.fd = _activeFd;
        pfd.events = POLLIN;

        int nbFd = poll(&pfd, 1, timeoutMs);

        if (nbFd > 0 && pfd.revents == POLLIN)
            return true;
        return false;
    }

    IPC& IPC::operator<<(const std::string& message) {
        if (_activeFd == -1) {
            throw std::runtime_error("IPC channel not configured for parent or child mode");
        }

        uint32_t len = message.size();
        ssize_t res;
        do {
            res = write(_activeFd, &len, sizeof(len));
        } while (res == -1 && errno == EINTR);

        if (res != sizeof(len)) {
            throw std::runtime_error("IPC write failed to send packet length");
        }
        
        if (len > 0) {
            const char* data = message.data();
            uint32_t written = 0;
            while (written < len) {
                do {
                    res = write(_activeFd, data + written, len - written);
                } while (res == -1 && errno == EINTR);

                if (res <= 0) {
                    throw std::runtime_error("IPC write failed to send packet data");
                }
                written += res;
            }
        }
        return *this;
    }

    IPC& IPC::operator>>(std::string& message) {
        if (_activeFd == -1) {
            throw std::runtime_error("IPC channel not configured for parent or child mode");
        }

        uint32_t len = 0;
        ssize_t res;
        do {
            res = read(_activeFd, &len, sizeof(len));
        } while (res == -1 && errno == EINTR);

        if (res == 0) {
            // EOF (connection closed gracefully)
            message.clear();
            return *this;
        } else if (res != sizeof(len)) {
            throw std::runtime_error("IPC read failed to receive packet length");
        }

        if (len == 0) {
            message.clear();
            return *this;
        }

        std::vector<char> buffer(len);
        uint32_t totalRead = 0;
        while (totalRead < len) {
            do {
                res = read(_activeFd, buffer.data() + totalRead, len - totalRead);
            } while (res == -1 && errno == EINTR);

            if (res <= 0) {
                throw std::runtime_error("IPC read failed to receive packet data");
            }
            totalRead += res;
        }

        message.assign(buffer.data(), len);
        return *this;
    }
}
