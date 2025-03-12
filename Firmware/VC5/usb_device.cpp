#include "VC5_global.h"



USBDevice g_usb_device;


void
init_usb()
{
    g_usb_device.init();
}


// ctor
USBDevice::USBDevice()
{
}


void
USBDevice::init()
{
    // create USB task
    task_handle_ = xTaskCreateStatic(                                                    
        task_entry,                                 // TaskFunction_t       pxTaskCode,
        task_usb::name,                             // const char * const   pcName,
        task_usb::c_stack_size_words,               // const uint32_t       ulStackDepth,
        this,                                       // void * const         pvParameters,
        task_usb::c_prio,                           // UBaseType_t          uxPriority,
        task_stack_,                                // StackType_t * const  puxStackBuffer,
        &task_TCB_                                  // StaticTask_t * const pxTaskBuffer
    );
}


void 
USBDevice::task_entry(void* param)
{
    USBDevice* This = static_cast<USBDevice*>(param);

    This->task();
}


void 
USBDevice::task()
{
    tusb_init();

    while(true)
    {
        tud_task_ext(task_usb::c_usb_task_interval_ms, false);

        // task for processing messages and sending a response
        vendor_1_task();

        // task for vendor 2, event interface
        vendor_2_task();
    }
}


void USBDevice::vendor_2_task()
{
    // check all rotaries
    for(uint i = 0; i < 5; i++)
    {
        // get value of rotary
        int rot_value = capture_rotary_value(i);
        if(rot_value != 0)
        {
            send_event_message(VC5_EventMsg::evt_rotary_turned, i, rot_value);

            break;
        }

        // get value of button
        int btn_value = capture_button_value(i);
        if(btn_value != 0)
        {
            send_event_message(VC5_EventMsg::evt_rotary_button, i, btn_value);

            break;
        }
    }
}


bool
USBDevice::send_event_message(uint8_t event_code, uint8_t index, int8_t value)
{
    // check if host has sent SET CONFIGURATION
    if(!tud_vendor_n_mounted_IN(ITF_INDEX_VENDOR_2))
    {
        return false;
    }

    // tud_vendor_n_write_available actually requires an index (0 based)
    if(CFG_TUD_VENDOR_TX_BUFSIZE == tud_vendor_n_write_available(ITF_INDEX_VENDOR_2))
    {
        // TX FiFo is empty
        // create message to send
        VC5_EventMsg msg;
        msg.event_code = event_code;
        msg.rotary_index = index;
        msg.value = value;
        msg.reserved = 0;

        static_assert(sizeof(msg) == VENDOR_INTERRUPT_EP_SIZE, "wrong msg size");
        // send message
        tud_vendor_n_write(ITF_INDEX_VENDOR_2, &msg, sizeof(msg));
        // for sending packages smaller than CFG_TUD_VENDOR_EPSIZE
        tud_vendor_n_flush(ITF_INDEX_VENDOR_2);

        return true;
    }

    return false;
}


// Invoked when a control transfer occurred
// Driver response accordingly to the request and the transfer stage (setup/data/ack)
// return false to stall control endpoint (e.g unsupported request)
extern "C"
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
    // nothing to with DATA & ACK stage
    if (stage != CONTROL_STAGE_SETUP) return true;

    if(request->bmRequestType_bit.type == TUSB_REQ_TYPE_VENDOR && request->bmRequestType_bit.recipient == TUSB_REQ_RCPT_DEVICE)
    {
        if(request->bmRequestType_bit.direction == TUSB_DIR_IN)
        {
            // Vendor request device IN
            switch (request->bRequest)
            {
                case VENDOR_REQUEST_MICROSOFT:
                    return get_ms_os_desc_request(rhport, request);
                default: 
                    break;
            }
        }
        else
        {
            // Vendor request device OUT
        }
    }

    // stall unknown request
    return false;
}
