#pragma once




class VC5Application
{
protected:
	// WinUSB
	WinUSBDevice device_;
	WinUSBInterface interface1_;
	WinUSBInterface interface2_;

	// for event data transmission
	VC5_EventMsg event_buffer_{0};

	// for bulk data transmission
	uint8_t unique_id_ {0};
	std::array<uint8_t, VC5_MsgHeader::max_cmd_length> cmd_buffer_ { 0 };
	std::array<uint8_t, VC5_MsgHeader::max_rsp_length> rsp_buffer_ { 0 };


	// VC5 units
	VC5Unit VC5_units_[c_num_units]
		{
			//led index,	display index,	unit name,	mute
			{0,				0,				L"Unit 0",	false}, // = Rotary 0
			{1,				1,				L"Unit 1",	false}, // = Rotary 1
			{2,				2,				L"Unit 2",	false}, // = Rotary 2
			{3,				3,				L"Unit 3",	false}, // = Rotary 3
			{4,				4,				L"Unit 4",	false}  // = Rotary 4 
		};

public:
	// ctor
	VC5Application();

	WinError
	init_WinUSB();

	WinError
	init_led();

	WinError
	init_displays();

	WinError
	check_event();

protected:
	WinError
	init_pipe_policies();

	WinError
	toggle_mute();

	WinError
	update_volume();

	// led index 0..4
	WinError
	update_led(const VC5Unit& unit);

	WinError
	update_display(const VC5Unit& unit);

	WinError
	clear_display(uint display_index);

	WinError
	print_string_on_display(uint8_t display_index, const char* string, uint8_t padding_top, uint8_t padding_left);

	void
	build_msg_set_color(const uint8_t led_index, const ColorTemplate& color);

	WinError
	build_msg_print_string(uint8_t display_index, const char* string, uint8_t padding_top, uint8_t padding_left);

	WinError
	transmit_and_check(uint msg_size);

	WinError
	process_rsp(size_t len_transferred);

	WinError
	print_vol_value(const VC5Unit& vc_unit);
};

