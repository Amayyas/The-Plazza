#include "../include/Thread.hpp"

namespace Plazza {
    Thread::~Thread() {
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void Thread::join() {
        if (_thread.joinable()) {
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
