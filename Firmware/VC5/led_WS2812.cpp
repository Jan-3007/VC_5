#include "VC5_global.h"

// PIO assembly program
#include "led_WS2812.pio.h"
#include "led_WS2812_pio_init.h"
#include "led_WS2812.h"



LedWS2812 g_WS2812_led_chain;



// ctor
LedWS2812::LedWS2812()
{
}


void 
LedWS2812::init()
{
    gpio_init(led_pin_);
    gpio_set_dir(led_pin_, GPIO_OUT);
    gpio_disable_pulls(led_pin_);
    gpio_set_function(led_pin_, GPIO_FUNC_PIO0);

    uint32_t offset = pio_add_program(pio_, &ws2812_program);
    ws2812_program_init(pio_, sm_, offset, led_pin_, (c_color_bit_depth * c_num_colors_per_led));

    // turn all LEDs off
    turn_all_off();
}


void 
LedWS2812::turn_all_off()
{
    color_array_.fill(0);
    send_to_leds();
}


void 
LedWS2812::send_to_leds()
{
    static_assert(c_num_leds <= 8, "FiFo cannot hold data of more than 8 LEDs");

    // wait for previous transfer to complete
    while(!pio_sm_is_tx_fifo_empty(pio_, sm_));

    // send a reset before data, 0 at least 50 us long, default value inside PIO is 0 so simply wait at least 50 us
    busy_wait_us_32(80);

    // send data to FiFo
    for(uint32_t i = 0; i < c_num_leds; i++)
    {
        uint32_t led_offset = i * c_bytes_per_led;

        uint32_t r = color_array_[led_offset + 0];
        uint32_t g = color_array_[led_offset + 1];
        uint32_t b = color_array_[led_offset + 2];

        uint32_t tx_data = (r << 24) | (g << 16) | (b << 8);

        pio_sm_put_blocking(pio_, sm_, tx_data);
    }
}


bool 
LedWS2812::set_color(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b)
{
    if(led_index >= 0 && led_index < c_num_leds)
    {
        uint32_t led_offset = led_index * c_bytes_per_led;

        color_array_[led_offset + 0] = r;
        color_array_[led_offset + 1] = g;
        color_array_[led_offset + 2] = b;
        return true;
    }

    return false;
}


void 
LedWS2812::test()
{
#if 0
    // pattern 1, 1 and 0 test
    set_color(0, 0xFF, 0xFF, 0xFF);   // test
    set_color(1, 0x00, 0x00, 0x00);   // test
    set_color(2, 0xFF, 0xFF, 0xFF);   // test
    set_color(3, 0x00, 0x00, 0x00);   // test
    set_color(4, 0xFF, 0xFF, 0xFF);   // test
#endif

#if 0
    // pattern 2, RGB, MSB test
    set_color(0, 0xFF, 0x00, 0x01);   // test
    set_color(1, 0x00, 0x00, 0x00);   // test
    set_color(2, 0x00, 0x00, 0x00);   // test
    set_color(3, 0x00, 0x00, 0x00);   // test
    set_color(4, 0x00, 0x00, 0x00);   // test
#endif

#if 0
    // pattern 3, RGB, 1 duration test
    set_color(0, 0xFF, 0xFF, 0xFF);   // test
    set_color(1, 0xFF, 0xFF, 0xFF);   // test
    set_color(2, 0xFF, 0xFF, 0xFF);   // test
    set_color(3, 0xFF, 0xFF, 0xFF);   // test
    set_color(4, 0xFF, 0xFF, 0xFF);   // test
#endif

#if 1
    set_color(0, 0xFF, 0xFF, 0xFF);   // white
    set_color(1, 0xFF, 0x00, 0x00);   // red
    set_color(2, 0x00, 0xFF, 0x00);   // lime
    set_color(3, 0x00, 0x00, 0xFF);   // blue
    set_color(4, 0xFF, 0xFF, 0x00);   // yellow
#endif

    send_to_leds();
}





void 
init_led_chain()
{
    g_WS2812_led_chain.init();

#if 0
    g_WS2812_led_chain.test();
#endif
}


bool 
set_single_led(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b)
{
    bool succ = g_WS2812_led_chain.set_color(led_index, r, g, b);
    if(succ)
    {
        g_WS2812_led_chain.send_to_leds();
    }
    return succ;
}
