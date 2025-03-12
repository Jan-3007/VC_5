#include "VC5_global.h"


void 
Rotary::init()
{
    mutex_.create();
}


void
Rotary::update(uint16_t pin_state)
{
    // true == High, false == Low
    bool current_clk_state = ((pin_state & clk_mask_) != 0);

    // detect falling edge
    if(prev_clk_state_ && !current_clk_state)
    {
        // true == High, false == Low
        bool data_state = ((pin_state & data_mask_) != 0);
        if(data_state)
        {
            if(reverse_cnt_dir_)
            {
                set_rot_value(-1);
            }
            else
            {
                set_rot_value(1);
            }
        }
        else
        {
            if(reverse_cnt_dir_)
            {
                set_rot_value(1);
            }
            else
            {
                set_rot_value(-1);
            }
        }
#if 0
        printf("R %d: %d\n", rotary_index_, capture_rot_value());
#endif
    }
    prev_clk_state_ = current_clk_state;



    // true == High == released, false == Low == pressed
    bool current_btn_state = ((pin_state & button_mask_) != 0);

    // detect press
    if(prev_btn_state_ && !current_btn_state)
    {
        // button pressed
        set_btn_value(1);
    }

    // detect release
    if(!prev_btn_state_ && current_btn_state)
    {
        // button released
        set_btn_value(-1);
    }
    
    prev_btn_state_ = current_btn_state;

#if 0
    if(capture_btn_value() != 0)
    {
        printf("B %d: %d\n", rotary_index_, capture_btn_value());
        button_value_ = 0;
    }
#endif
}


void 
Rotary::set_rot_value(int value)
{
    ScopedMutex lock{mutex_};
    rotary_value_ += value;
}


int 
Rotary::capture_rot_value()
{
    ScopedMutex lock{mutex_};

    int temp = rotary_value_;
    rotary_value_ = 0;

    return temp;
}


void 
Rotary::set_btn_value(int value)
{
    ScopedMutex lock{mutex_};

    button_value_ = value;
}


int 
Rotary::capture_btn_value()
{
    ScopedMutex lock{mutex_};

    int temp = button_value_;
    button_value_ = 0;
    return temp;
}
