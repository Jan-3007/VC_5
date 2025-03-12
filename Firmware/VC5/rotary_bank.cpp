#include "VC5_global.h"



RotaryBank g_rotary_bank;


void 
init_rotaries()
{
    g_rotary_bank.init();
}


void 
mcp_isr_callback(uint gpio, uint32_t event_mask)
{
    g_rotary_bank.notify_task();
}


// ctor
RotaryBank::RotaryBank()
{

}


void 
RotaryBank::init()
{
    for(Rotary& rot : rotary_)
    {
        rot.init();
    }

    create_task();
}


void 
RotaryBank::update_rotaries(uint16_t current_state)
{
    // update each rotary value based on rotation direction
    for(Rotary& rot : rotary_)
    {
        rot.update(current_state);
    }
}


void 
RotaryBank::create_task()
{
    task_handle_ = xTaskCreateStatic(                                                    
        task_entry,                                 // TaskFunction_t       pxTaskCode,
        task_rotaries::name,                        // const char * const   pcName,
        task_rotaries::c_stack_size_words,          // const uint32_t       ulStackDepth,
        this,                                       // void * const         pvParameters,
        task_rotaries::c_prio,                      // UBaseType_t          uxPriority,
        task_stack_,                                // StackType_t * const  puxStackBuffer,
        &task_TCB_                                  // StaticTask_t * const pxTaskBuffer
    );
}


// static
void 
RotaryBank::task_entry(void* param)
{
    RotaryBank* This = static_cast<RotaryBank*>(param);

    This->task();
}


void 
RotaryBank::task_startup()
{
    mcp_.init();

    gpio_set_dir(c_mcp_int_pin, false);

    // enable pull-up for the interrupt pin on the pico
    // gpio_set_pulls(c_mcp_int_pin, true, false);

    // set up interrupt routine on the pico
    gpio_set_irq_enabled_with_callback(c_mcp_int_pin, GPIO_IRQ_EDGE_FALL, true, &mcp_isr_callback);

    // dummy read to clear interrupt
    mcp_.get_pin_state();

    // enable interrupt on MCP23017
    mcp_.enable_interrupt(); 
}


void 
RotaryBank::task()
{
    task_startup();

    while(true)
    {
        // set task to wait to be activated by notification
        uint32_t val = ulTaskNotifyTake(              
            pdTRUE,                             // reset notification value to 0
            portMAX_DELAY                       // wait indefinitely, no timeout
        );

        uint16_t current = mcp_.get_pin_state();
        uint16_t prev = current;
        uint cnt = 0;

        // debpounce pin states
        while(cnt < c_debounce_threshold)
        {
            vTaskDelay(ms_to_ticks(c_debounce_delay_ms));

            current = mcp_.get_pin_state();

            if(current == prev)
            {
                cnt++;
            }
            else
            {
                cnt = 0;
                prev = current;
            }
        }

        // pin state stable
//        printf("Debounced pin state: 0x%04X\n", current);

        // update each rotary value based on rotation direction
        update_rotaries(current);
    }
}


int 
RotaryBank::capture_rot_value(uint index)
{
    return rotary_[index].capture_rot_value();
}


int 
RotaryBank::capture_btn_value(uint index)
{
    return rotary_[index].capture_btn_value();
}


int
capture_rotary_value(uint index)
{
    assert(index < c_num_rotaries);
    return g_rotary_bank.capture_rot_value(index);
}


int
capture_button_value(uint index)
{
    assert(index < c_num_rotaries);
    return g_rotary_bank.capture_btn_value(index);
}


