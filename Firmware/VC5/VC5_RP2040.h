#pragma once


//--------------------
// I²C
//--------------------
void 
init_i2c();

int 
write_i2c(uint8_t addr, const uint8_t* src, size_t len, bool nostop);

int 
read_i2c(uint8_t addr, uint8_t* dst, size_t len, bool nostop);



//--------------------
// SPI
//--------------------
void 
init_spi();

void 
set_spi_dc_pin(bool value);

void 
set_spi_cs_pin(uint32_t cs_pin, bool value);

void 
set_spi_reset_pin(bool value);

int 
write_spi(const uint8_t* src, size_t len);
