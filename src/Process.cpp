#include "../include/Process.hpp"
#include <csignal>
#include <sys/wait.h>

namespace Plazza {

    Process::~Process() {
        wait();
    }

    Process::Process(Process&& other) noexcept : _pid(other._pid) {
        other._pid = -1;
    }

    Process& Process::operator=(Process&& other) noexcept {
        if (this != &other) {
            wait();
            _pid = other._pid;
            other._pid = -1;
        }
        return *this;
    }

    void Process::wait() {
        if (_pid > 0) {
            waitpid(_pid, nullptr, 0);
            _pid = -1;
        }
    }

    bool Process::isAlive() const {
        if (_pid <= 0) {
            return false;
        }
        // kill(pid, 0) checks if the process exists without sending a signal
        return kill(_pid, 0) == 0;
    }

    pid_t Process::getPid() const {
        return _pid;
    }

}
