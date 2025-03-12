#pragma once




class RotaryBank
{
protected:
    MCP23017 mcp_;

    Rotary rotary_[c_num_rotaries]
        {
        //   num, clk,          data,            button             reverse_cnt
            {0, rot_0.clk_mask, rot_0.data_mask, rot_0.button_mask, false},
            {1, rot_1.clk_mask, rot_1.data_mask, rot_1.button_mask, false},
            {2, rot_2.clk_mask, rot_2.data_mask, rot_2.button_mask, false},
            {3, rot_3.clk_mask, rot_3.data_mask, rot_3.button_mask, false},
            {4, rot_4.clk_mask, rot_4.data_mask, rot_4.button_mask, false},
        };

    TaskHandle_t task_handle_;
    StaticTask_t task_TCB_;
    StackType_t task_stack_[task_rotaries::c_stack_size_words];


public:
    // ctor
    RotaryBank();

    void 
    init();

    void 
    notify_task()
    {
        vTaskNotifyGiveFromISR(task_handle_, NULL);
    }

    int
    capture_rot_value(uint index);

    int
    capture_btn_value(uint index);

protected:
    void 
    create_task();

    static void 
    task_entry(void* param);

    void 
    task_startup();

    void 
    task();

    void 
    update_rotaries(uint16_t current_state);

};


void 
init_rotaries();

int
capture_rotary_value(uint index);

int
capture_button_value(uint index);


