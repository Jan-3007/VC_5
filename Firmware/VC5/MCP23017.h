#pragma once


class MCP23017
{
protected:
    static constexpr uint8_t slave_address = 0x27;

    // bitmask for rotary CLK pins
    static const uint16_t clk_pin_mask_ = 0
                | rot_0.clk_mask
                | rot_1.clk_mask
                | rot_2.clk_mask
                | rot_3.clk_mask
                | rot_4.clk_mask;

    // bitmask for rotary CLK pins
    static const uint16_t button_pin_mask_ = 0
                | rot_0.button_mask
                | rot_1.button_mask
                | rot_2.button_mask
                | rot_3.button_mask
                | rot_4.button_mask;


public:
    // ctor
    MCP23017();
    
    // Initialise MCP23017
    void 
    init();

    // bit 0..7: Port A
    // bit 8..15: Port B
    uint16_t 
    get_pin_state();

    void 
    enable_interrupt();

protected:
    // function to write a 16 bit value to a register of MCP23017 via I²C
    bool 
    write_register16(uint8_t reg_addr, uint16_t value);

    // function to read a 16 bit value to a register of MCP23017 via I²C
    bool 
    read_register16(uint8_t reg_addr, uint16_t& value);

    void 
    configure_int();
};

