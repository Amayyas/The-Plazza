#include "../include/ConditionVariable.hpp"

namespace Plazza {
    void ConditionVariable::wait(Mutex& mutex) {
        std::unique_lock<std::mutex> lock(mutex.getNative(), std::adopt_lock);
        try {
            _cv.wait(lock);
            lock.release();
        } catch (...) {
            lock.release();
            throw;
        }
    }

    void ConditionVariable::notifyOne() {
        _cv.notify_one();
    }

    void ConditionVariable::notifyAll() {
        _cv.notify_all();
    }
}
