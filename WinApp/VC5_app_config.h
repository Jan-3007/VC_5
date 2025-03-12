#pragma once


//-----------------------------------------------------------
// VC 5 - General
//-----------------------------------------------------------

constexpr uint32_t c_num_units = 5;         // 1 unit = 1 rotary + 1 display + 1 LED
constexpr uint32_t c_num_rotaries = c_num_units;
constexpr uint32_t c_num_leds = c_num_units;
constexpr uint32_t c_num_displays = c_num_units;



//-----------------------------------------------------------
// VC 5 - User preferences
//-----------------------------------------------------------

// true: if volume is changed, mute will be set according to the new value
// false: mute remains when volume is changed
constexpr bool c_clear_mute_on_change = false;

// LED color for Mute state
constexpr ColorTemplate mute_off = color::green;
constexpr ColorTemplate mute_on = color::red;

// Display lines
namespace disp_lines
{
	constexpr uint name_line_num = LinesFont12::LINE_1;
	constexpr uint name_padding_left = 0;
	constexpr uint volume_line_num = LinesFont12::LINE_3;
	constexpr uint volume_padding_left = 10;
}