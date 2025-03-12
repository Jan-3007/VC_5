#pragma once


class USBDevice
{
protected:
    TaskHandle_t task_handle_;
    StaticTask_t task_TCB_;
    StackType_t task_stack_[task_usb::c_stack_size_words];


public:
    // ctor
    USBDevice();

    void
    init();

protected:
    static void 
    task_entry(void* param);

    void 
    task();

    void
    vendor_2_task();

    bool
    send_event_message(uint8_t event_code, uint8_t index, int8_t value);
};



void
init_usb();
