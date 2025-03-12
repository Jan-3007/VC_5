#pragma once



class LedWS2812
{
protected:
    // color order for each led is RGB (R: LSB, B: MSB)
    std::array<uint8_t, (c_bytes_per_led * c_num_leds)> color_array_ {0};

    // required for PIO
    PIO pio_ {pio0};
    uint32_t sm_ {0};
    uint32_t led_pin_ {c_led_pin};

public:
    // ctor
    LedWS2812();

    void
    init();

    // turn all LEDs off
    void
    turn_all_off();

    void
    send_to_leds();

    // led index 0 .. 4
    // returns false if index is out of range
    bool
    set_color(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b);

    void
    test();
};


void
init_led_chain();

// led index 0 .. 4
// returns false if index is out of range
bool
set_single_led(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b);