#pragma once


// Mutex abstraction
// recursive mutex with priority inheritance
class Mutex
{
protected:
    SemaphoreHandle_t mutex_ {nullptr};
    StaticSemaphore_t mutex_memory_;

public:
    // ctor
    Mutex();

    void
    create();

    // blocks indefinitely
    void
    aquire();

    void
    release();

    void
    destroy();

    bool
    is_valid() const
        {
            return (mutex_ != nullptr);
        }
};


class ScopedMutex
{
protected:
    Mutex& mutex_;

public:
    // ctor
    ScopedMutex(Mutex& mutex)
        : mutex_{mutex}
        {
            mutex.aquire();
        }

    // dtor
    ~ScopedMutex()
        {
            mutex_.release();
        }
};