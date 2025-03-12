#pragma once



//-----------------------------------------------------------
// VC 5 - General
//-----------------------------------------------------------

constexpr uint32_t c_num_units = 5;         // 1 unit = 1 rotary + 1 display + 1 LED



//-----------------------------------------------------------
// Raspberry Pi Pico - Pins
//-----------------------------------------------------------

// Rotaries
constexpr uint32_t c_i2c_sda_pin = 6;
constexpr uint32_t c_i2c_scl_pin = 7;
constexpr uint32_t c_mcp_int_pin = 3;

// LEDs
constexpr uint32_t c_led_pin = 20;

// Displays
constexpr int c_spi_data_pin = 19;
constexpr int c_spi_clk_pin = 18;
constexpr int c_spi_dc_pin = 16;
constexpr int c_disp_reset_pin = 10;

constexpr int c_spi_cs0_pin = 12;
constexpr int c_spi_cs1_pin = 13;
constexpr int c_spi_cs2_pin = 14;
constexpr int c_spi_cs3_pin = 17;
constexpr int c_spi_cs4_pin = 15;




//-----------------------------------------------------------
// Raspberry Pi Pico - I²C
//-----------------------------------------------------------

#define I2C_PORT i2c1
// freq = 400 kHz
constexpr int32_t c_i2c_freq_hz = 400 * 1000;


//-----------------------------------------------------------
// Raspberry Pi Pico - SPI
//-----------------------------------------------------------
#define SPI_PORT spi0
// freq = 1 MHz
constexpr int c_spi_freq_hz = 1000 * 1000;



//-----------------------------------------------------------
// FreeRTOS
//-----------------------------------------------------------

// RP2040 SRAM size: 264kB on-chip
// Pico Flash size: 2MB on-board

namespace task_rotaries
{
    const char name[] = "ROT";
    constexpr uint32_t c_prio = configMAX_PRIORITIES - 1u;
    constexpr uint32_t c_stack_size_bytes = 1024;
    constexpr uint32_t c_stack_size_words = c_stack_size_bytes / 4;
}

namespace task_usb
{
    const char name[] = "USB";
    constexpr uint32_t c_prio = configMAX_PRIORITIES - 2u;
    constexpr uint32_t c_stack_size_bytes = 4096;
    constexpr uint32_t c_stack_size_words = c_stack_size_bytes / 4;

    constexpr uint32_t c_usb_task_interval_ms = 10;
}



//-----------------------------------------------------------
// Rotaries + MCP23017
//-----------------------------------------------------------

struct rotary_pins_mask
{
    uint16_t clk_mask {0};
    uint16_t data_mask {0};
    uint16_t button_mask {0};
};

//
// rotary masks
//                               clk,       data,      button
constexpr rotary_pins_mask rot_0 {(1u << 1), (1u << 0), (1u << 2)};
constexpr rotary_pins_mask rot_1 {(1u << 4), (1u << 5), (1u << 3)};
constexpr rotary_pins_mask rot_2 {(1u << 15), (1u << 7), (1u << 6)};
constexpr rotary_pins_mask rot_3 {(1u << 10), (1u << 9), (1u << 14)};
constexpr rotary_pins_mask rot_4 {(1u << 12), (1u << 11), (1u << 13)};


constexpr uint32_t c_num_rotaries = c_num_units;


constexpr uint32_t c_debounce_threshold = 2;
constexpr uint32_t c_debounce_delay_ms = 1;



//-----------------------------------------------------------
// WS2812 LEDs
//-----------------------------------------------------------

constexpr uint32_t c_num_leds = c_num_units;

// RGB, 8 bit for each color 
constexpr uint32_t c_color_bit_depth = 8;
constexpr uint32_t c_num_colors_per_led = 3;
constexpr uint32_t c_bytes_per_led = (c_color_bit_depth * c_num_colors_per_led) / 8;



//-----------------------------------------------------------
// Waveshare 1.3 inch OLED blue on black display
//-----------------------------------------------------------

constexpr uint32_t c_num_displays = c_num_units;

struct display_properties
{
    uint32_t cs_pin;
    uint32_t width;
    uint32_t height;
};


#define USE_WAVESHARE_1IN3_OLED
#ifdef USE_WAVESHARE_1IN3_OLED

// different display sizes is not supported
constexpr uint32_t c_display_width = 128;
constexpr uint32_t c_display_height = 64;
constexpr uint32_t c_display_size = c_display_width * c_display_height;

//                                  cs_pin,             width,              height
constexpr display_properties disp_0 {c_spi_cs0_pin,     c_display_width,    c_display_height};
constexpr display_properties disp_1 {c_spi_cs1_pin,     c_display_width,    c_display_height};
constexpr display_properties disp_2 {c_spi_cs2_pin,     c_display_width,    c_display_height};
constexpr display_properties disp_3 {c_spi_cs3_pin,     c_display_width,    c_display_height};
constexpr display_properties disp_4 {c_spi_cs4_pin,     c_display_width,    c_display_height};

// default image rotation 
constexpr uint16_t c_image_rotation = ROTATE_270;
// default font for text
constexpr sFONT* c_display_font = &Font12;

#endif



