#pragma once

#include "led_WS2812.pio.h"



static inline 
void ws2812_program_init(
    PIO pio, 
    uint32_t sm, 
    uint32_t offset, 
    uint32_t pin,
    uint32_t bit_depth
    )
{
    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);

    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_out_shift(
        &c, 
        false,                  // false for MSB first
        true,                   // true for autopull (sm automatically refills the OSR from TX FiFo without wasting cycles)
        bit_depth               // number of bits the sm triggers a refill
        );

    // this allows for a 8 * 32 bit TX FiFo
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    // to slow down the sm's execution
    // one sm cycle == 16 ns
    sm_config_set_clkdiv(&c, 2.0);

    // Load configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);

    // clear the FiFos
    pio_sm_clear_fifos(pio, sm);

    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}
