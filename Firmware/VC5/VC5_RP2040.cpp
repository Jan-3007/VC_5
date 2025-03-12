#include "VC5_global.h"
#include "VC5_RP2040.h"



//--------------------
// I²C
//--------------------
void 
init_i2c()
{
    // I2C Initialisation
    i2c_init(I2C_PORT, c_i2c_freq_hz);
    
    gpio_set_function(c_i2c_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(c_i2c_scl_pin, GPIO_FUNC_I2C);
    //gpio_pull_up(c_i2c_sda_pin);
    //gpio_pull_up(c_i2c_scl_pin);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
}


int 
write_i2c(uint8_t addr, const uint8_t* src, size_t len, bool nostop)
{
    return i2c_write_blocking(I2C_PORT, addr, src, len, nostop);
}


int 
read_i2c(uint8_t addr, uint8_t* dst, size_t len, bool nostop)
{
    return i2c_read_blocking(I2C_PORT, addr, dst, len, nostop);
}



//--------------------
// SPI
//--------------------
void 
init_spi()
{
    // SPI initialization at 1MHz
    spi_init(SPI_PORT, c_spi_freq_hz);

    gpio_set_function(c_spi_clk_pin, GPIO_FUNC_SPI);
    gpio_set_function(c_spi_data_pin, GPIO_FUNC_SPI);

    // Chip select is active-low, initialize it to inactive-high
    gpio_init(disp_0.cs_pin);
    gpio_set_dir(disp_0.cs_pin, GPIO_OUT);
    gpio_put(disp_0.cs_pin, true);

    gpio_init(disp_1.cs_pin);
    gpio_set_dir(disp_1.cs_pin, GPIO_OUT);
    gpio_put(disp_1.cs_pin, true);

    gpio_init(disp_2.cs_pin);
    gpio_set_dir(disp_2.cs_pin, GPIO_OUT);
    gpio_put(disp_2.cs_pin, true);

    gpio_init(disp_3.cs_pin);
    gpio_set_dir(disp_3.cs_pin, GPIO_OUT);
    gpio_put(disp_3.cs_pin, true);

    gpio_init(disp_4.cs_pin);
    gpio_set_dir(disp_4.cs_pin, GPIO_OUT);
    gpio_put(disp_4.cs_pin, true);

    // init of pin D/C
    gpio_init(c_spi_dc_pin);
    gpio_set_dir(c_spi_dc_pin, GPIO_OUT);
    gpio_put(c_spi_dc_pin, false);

    // init of pin reset, set active-low on startup, see datasheet p. 32
    gpio_init(c_disp_reset_pin);
    gpio_set_dir(c_disp_reset_pin, GPIO_OUT);
    gpio_put(c_disp_reset_pin, false);
}


void 
set_spi_dc_pin(bool value)
{
    gpio_put(c_spi_dc_pin, value);
}


void 
set_spi_cs_pin(uint32_t cs_pin, bool value)
{
    gpio_put(cs_pin, value);
}


void 
set_spi_reset_pin(bool value)
{
    gpio_put(c_disp_reset_pin, value);
}


int 
write_spi(const uint8_t *src, size_t len)
{
    return spi_write_blocking(SPI_PORT, src, len);
}
