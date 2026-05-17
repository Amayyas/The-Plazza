#include "../include/ConditionVariable.hpp"

namespace Plazza {
    void ConditionVariable::wait(Mutex& mutex) {
        std::unique_lock<std::mutex> lock(mutex.getNative(), std::adopt_lock);
        _cv.wait(lock);
        lock.release();
    }

    void ConditionVariable::notifyOne() {
        _cv.notify_one();
    }

    void ConditionVariable::notifyAll() {
        _cv.notify_all();
    }
}
