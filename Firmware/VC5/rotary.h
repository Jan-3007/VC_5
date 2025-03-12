#pragma once



class Rotary
{
protected:
    // mutex protects rotary and button values
    Mutex mutex_;
    int rotary_value_ {0};
    int button_value_ {0};

    // parameters
    uint rotary_index_;
    uint16_t clk_mask_;
    uint16_t data_mask_;
    uint16_t button_mask_;

    // internal state
	bool prev_clk_state_ {true};
    int prev_btn_state_ {true};
	bool reverse_cnt_dir_ {false};      

public:
    // ctor
    Rotary(uint index, uint16_t clk, uint16_t data, uint16_t button, bool reverse)
        : rotary_index_ {index}, clk_mask_ {clk}, data_mask_ {data}, button_mask_ {button}, reverse_cnt_dir_ {reverse}
        {}

    void
    init();

    void 
    update(uint16_t pin_state);

    void
    set_rot_value(int value);

    int
    capture_rot_value();

    void
    set_btn_value(int value);

    int
    capture_btn_value();

};






