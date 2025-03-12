#include "VC5_global.h"


// ctor
MCP23017::MCP23017()
{}


void 
MCP23017::init()
{
    // enable weak pull-up resistors on each pin
    write_register16(0x0C, 0xFFFF);

    configure_int();
}


uint16_t 
MCP23017::get_pin_state()
{
    uint16_t value;
    if(read_register16(0x12, value))
    {
        return value;
    }

    return 0;
}


bool 
MCP23017::write_register16(uint8_t reg_addr, uint16_t value)
{
    // data[0] = reg_addr
    // data[1] = Port A
    // data[2] = Port B
    uint8_t data[3] = {
        reg_addr,
        static_cast<uint8_t>(value & 0x00FF),
        static_cast<uint8_t>((value>>8) & 0x00FF)
        };        

    int n = write_i2c(slave_address, data, 3, false);

    if(n < 0)
    {
        printf("write_register16: write_i2c failed, returned %d\n", n);
        return false;
    }

    return true;
}


// takes about 150 us to complete (@400kHz)
bool 
MCP23017::read_register16(uint8_t reg_addr, uint16_t& value)
{
    bool success = true;

    int n = write_i2c(slave_address, &reg_addr, 1, true);
    if(n < 0)
    {
        printf("read_register16: write_i2c failed, returned %d\n", n);
        success = false;
    }

    // data[0] = Port A
    // data[1] = Port B
    uint8_t data[2] = {0};        
    n = read_i2c(slave_address, data, 2, false);
    if(n < 0)
    {
        printf("read_register16: read_i2c failed, returned %d\n", n);
        success = false;
    }
    
    if(success)
    {
        value = data[1];
        value <<= 8;
        value |= data[0];
    }
    
    return success;
}


void 
MCP23017::enable_interrupt()
{
    // GPINTEN
    // enable interrupt-on-change feature on selected pins
    write_register16(0x04, clk_pin_mask_ | button_pin_mask_);
}


void 
MCP23017::configure_int()
{
    // INTCON
    // pin value will be compared against the previous state
    write_register16(0x08, 0x0000);

    // enable IOCON.MIRROR
    // addresses of the registers of port A and B are set to sequential (BANK), refer to tables labelled with BANK = 0
    // interrupt pins are mirrored (MIRROR), interrupts from all GPIO ports will be passed to both interrupt pins
    // set interrupt pin as open drain (ODR)
    write_register16(0x0A, 0x4040);
}

