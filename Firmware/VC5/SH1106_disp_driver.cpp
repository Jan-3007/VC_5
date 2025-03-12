#include "VC5_global.h"
#include "SH1106_disp_driver.h"



void SH1106DisplayDriver::init()
{
    hw_reset();

    init_controller();

    clear_screen();

    display_on();

    // see SH1106 datasheet, p.32
    busy_wait_ms(100);
}


DisplayDriverITF::DisplayProperties 
SH1106DisplayDriver::get_display_properties()
{
    return DisplayProperties {width_, height_};
}


void 
SH1106DisplayDriver::display_on()
{
    send_cmd(0xAF);
}


void 
SH1106DisplayDriver::display_off()
{
    send_cmd(0xAE);
}


bool 
SH1106DisplayDriver::clear_screen()
{
    for(uint8_t page = 0; page < (height_ / 8); page++)
    {
        // set page address 
        send_cmd(0xB0 + page);
        // set low column address 
        send_cmd(0x02);
        // set high column address
        send_cmd(0x10);

        for(uint8_t column = 0; column < width_; column++)
        {
            bool succ = send_data('\0');
            if(!succ)
            {
                return false;
            }
        }
    }
    return true;
}


bool 
SH1106DisplayDriver::send_image(const uint8_t *image, uint32_t image_size)
{
    for(uint8_t page = 0; page < (height_ / 8); page++)
    {
        // set page address 
        send_cmd(0xB0 + page);
        // set low column address 
        send_cmd(0x02);
        // set high column address
        send_cmd(0x10);

        for(uint8_t column = 0; column < width_; column++)
        {
            bool succ = send_data(image[(7 - page) + (column * 8)]);
            if(!succ)
            {
                return false;
            }
        }
    }
    return true;
}


void 
SH1106DisplayDriver::hw_reset()
{
    // release reset with delay, see datasheet p.32
    busy_wait_us(10);
    set_spi_reset_pin(true);
}

void 
SH1106DisplayDriver::init_controller()
{
    display_off();

    send_cmd(0x02);//---set low column address
    send_cmd(0x10);//---set high column address

    send_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    send_cmd(0x81);//--set contrast control register
    send_cmd(0xA0);//--Set SEG/Column Mapping a0/a1
    send_cmd(0xC0);//Set COM/Row Scan Direction
    send_cmd(0xA6);//--set normal display a6/a7
    send_cmd(0xA8);//--set multiplex ratio(1 to 64)
    send_cmd(0x3F);//--1/64 duty
    send_cmd(0xD3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    send_cmd(0x00);//-not offset
    send_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    send_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    send_cmd(0xD9);//--set pre-charge period
    send_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    send_cmd(0xDA);//--set com pins hardware configuration
    send_cmd(0x12);
    send_cmd(0xDB);//--set vcomh
    send_cmd(0x40);//Set VCOM Deselect Level
    send_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    send_cmd(0x02);//
    send_cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
    send_cmd(0xA6);// Disable Inverse Display On (0xa6/a7)
}


void 
SH1106DisplayDriver::send_cmd(uint8_t cmd)
{
    // set DC = 0
    set_spi_dc_pin(false);
    // set CS = 0
    set_spi_cs_pin(cs_pin_, false);
    // send reg byte
    write_spi(&cmd, 1);
    // set CS = 1
    set_spi_cs_pin(cs_pin_, true);
}


bool 
SH1106DisplayDriver::send_data(uint8_t data)
{
    // set DC = 1
    set_spi_dc_pin(true);
    // set CS = 0
    set_spi_cs_pin(cs_pin_, false);
    // send data byte
    uint8_t written = write_spi(&data, 1);
    if(written != 1)
    {
        return false;
    }
    // set CS = 1
    set_spi_cs_pin(cs_pin_, true);
    return true;
}

