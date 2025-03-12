#include "VC5_global.h"


// ctor
Mutex::Mutex()
{

}


void
Mutex::create()
{
    assert(mutex_ == nullptr);
    mutex_ = xSemaphoreCreateRecursiveMutexStatic(&mutex_memory_);
    assert(mutex_ != nullptr);
}


void
Mutex::aquire()
{
    // block indefinitely
    xSemaphoreTakeRecursive(mutex_, portMAX_DELAY);
}


void
Mutex::release()
{
    xSemaphoreGiveRecursive(mutex_);
}


void Mutex::destroy()
{
    if(mutex_ != nullptr)
    {
        vSemaphoreDelete(mutex_);
        mutex_ = nullptr;
    }
}

