#include "VC5_global.h"


// ctor
VC5Application::VC5Application()
{

}


WinError 
VC5Application::init_WinUSB()
{
	GUID device_itf_guid = VC5_WINUSB_INTERFACE_GUID_INIT;

	// step 1: enumerate WinUSB devices, filter with device guid
	std::vector<std::wstring> device_path_list;
	WinError err = enumerate_devices(device_itf_guid, device_path_list);
	if (err != NO_ERROR)
	{
		return err;
	}
	if (device_path_list.empty())
	{
		std::cout << std::format("{}: No devices found.\n", __func__);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// step 2: open device
	err = device_.open(device_path_list[0].c_str());
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 3: get default interface, interface 0
	err = interface1_.init(device_.get_device_handle());
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 4: get second interface, first associated itf is at pos 0
	err = interface2_.get_associated_interface(interface1_.get_itf_handle(), 0);
	if (err != NO_ERROR)
	{
		return err;
	}

	// step 5: init pipe policies and remove any old data from device IN pipe fifo
	err = init_pipe_policies();
	if (err != NO_ERROR)
	{
		return err;
	}

	return NO_ERROR;
}


WinError 
VC5Application::init_led()
{
	WinError err;
	for (uint8_t i = 0; i < c_num_leds; i++)
	{
		err = update_led(VC5_units_[i]);
		if (err != NO_ERROR)
		{
			return err;
		}
	}
	return err;
}


WinError 
VC5Application::init_displays()
{
	WinError err = NO_ERROR;
	for (int i = 0; i < c_num_displays; i++)
	{
		err = clear_display(i);
		if (err != NO_ERROR)
		{
			// unexpected error
			std::cout << std::format("{}: could not clear display\n", __func__);
			break;
		}
		
		err = update_display(VC5_units_[i]);
		if (err != NO_ERROR)
		{
			// unexpected error
			std::cout << std::format("{}: could not update display\n", __func__);
			break;
		}
	}

	return err;
}


WinError 
VC5Application::init_pipe_policies()
{
	WinError err;

	err = interface2_.set_pipe_transfer_timeout(EPNUM_VENDOR_2_INT_IN, 100);
	if (err != NO_ERROR)
	{
		return err;
	}

	// TODO: remove any old data from device IN pipe fifo

	return NO_ERROR;
}


WinError 
VC5Application::check_event()
{
	// read pipe
	size_t len_transferred = 0;
	WinError err = interface2_.read_pipe_sync(EPNUM_VENDOR_2_INT_IN, &event_buffer_, sizeof(event_buffer_), len_transferred);
	if (err != NO_ERROR)
	{
		return err;
	}

	if (len_transferred == 0)
	{
		// read timed out
		return NO_ERROR;
	}

	if (len_transferred != sizeof(VC5_EventMsg))
	{
		// unexpected length
		std::cout << std::format("{}: Vendor 2: unexpected length = {}\n", __func__, len_transferred);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// check rotary index
	if (event_buffer_.rotary_index >= c_num_rotaries)
	{
		// not a valid index
		std::cout << std::format("{}: Vendor 2: unexpected index = {}\n", __func__, event_buffer_.rotary_index);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// process data from pipe
//	std::cout << std::format("Event received: Code = {}, Rot index = {}, value = {}\n",
//		event_buffer_.event_code, event_buffer_.rotary_index, event_buffer_.value);

	err = NO_ERROR;
	switch (event_buffer_.event_code)
	{
		case VC5_EventMsg::evt_rotary_turned:
			// on rotary turned
			if(event_buffer_.value != 0)
			{
				err = update_volume();
			}
			break;

	
		case VC5_EventMsg::evt_rotary_button:
			// on button released
			if (event_buffer_.value == -1)
			{
				err = toggle_mute();
			}
			else
			{
				//std::cout << std::format("btn pressed\n");
			}
			break;


		default:
			// unknown event code
			std::cout << std::format("{}: Vendor 2: unknown event code = {}\n", __func__, event_buffer_.event_code);
			return ERROR_ERRORS_ENCOUNTERED;
	}

	return err;
}


WinError 
VC5Application::toggle_mute()
{
	VC5Unit& unit = VC5_units_[event_buffer_.rotary_index];

	if (unit.is_mute())
	{
		// clear mute
		unit.set_mute(false);
	}
	else
	{
		// set mute
		unit.set_mute(true);
	}

	std::cout << std::format("{}: rot = {}, volume = {}, mute = {}\n", __func__, event_buffer_.rotary_index, unit.get_volume(), unit.is_mute());

	return update_led(unit);
}


WinError 
VC5Application::update_volume()
{
	WinError err = NO_ERROR;

	VC5Unit& unit = VC5_units_[event_buffer_.rotary_index];

	// increase or decrease volume
	unit.change_volume(event_buffer_.value);

	if(c_clear_mute_on_change)
	{
		// update mute after volume change
		unit.update_mute();

		err = update_led(unit);
		if (err != NO_ERROR)
		{
			return err;
		}
	}

	std::cout << std::format("{}: rot = {}, new volume = {}, mute = {}\n", __func__, event_buffer_.rotary_index, unit.get_volume(), unit.is_mute());

	err = update_display(unit);

	return err;
}


WinError 
VC5Application::update_led(const VC5Unit& unit)
{
	build_msg_set_color(unit.get_led_index(), unit.get_led_color());

	uint msg_size = sizeof(VC5_MsgHeader) + sizeof(VC5_SetLed);
	return transmit_and_check(msg_size);
}


WinError 
VC5Application::update_display(const VC5Unit& unit)
{
	WinError err;

	// print app name on the display
	std::wstring name = unit.get_name();

	char disp_name[VC5_PrintString::max_character];
	std::fill(disp_name, disp_name + VC5_PrintString::max_character, ' ');

	// the char array is length limited to fit in one line of the display
	int name_size = 0;
	if (name.size() <= std::size(disp_name))
	{
		name_size = static_cast<int>(name.size());
	} 
	else
	{
		name_size = static_cast<int>(std::size(disp_name));
	}

	// convert wstring to char array
	err = convert_wstring_to_char(name, name_size, disp_name, std::size(disp_name));
	if (err != NO_ERROR)
	{
		return err;
	}

	// the string needs to be zero-terminated
	disp_name[VC5_PrintString::max_character - 1] = '\0';

	err = print_string_on_display(unit.get_display_index(), disp_name, disp_lines::name_line_num, disp_lines::name_padding_left);
	if (err != NO_ERROR)
	{
		return err;
	}

	// print volume value on the display
	err = print_vol_value(unit);

	return err;

}


WinError 
VC5Application::clear_display(uint display_index)
{
	unique_id_++;

	VC5_MsgHeader* header = reinterpret_cast<VC5_MsgHeader*>(cmd_buffer_.data());
	header->message_length = sizeof(VC5_MsgHeader) + sizeof(VC5_ClearDisplay);
	header->message_code = VC5_MsgHeader::cmd_clear_display;
	header->unique_id = unique_id_;
	header->message_status = 0;
	header->reserved = 0;

	VC5_ClearDisplay* data = reinterpret_cast<VC5_ClearDisplay*>(cmd_buffer_.data() + sizeof(VC5_MsgHeader));
	data->display_index = display_index;

	unsigned int msg_size = sizeof(VC5_MsgHeader) + sizeof(VC5_ClearDisplay);
	return transmit_and_check(msg_size);
}


WinError
VC5Application::print_string_on_display(uint8_t display_index, const char* string, uint8_t padding_top, uint8_t padding_left)
{
	WinError err = build_msg_print_string(display_index, string, padding_top, padding_left);
	if (err != NO_ERROR)
	{
		return err;
	}
	unsigned int msg_size = sizeof(VC5_MsgHeader) + sizeof(VC5_PrintString);
	return transmit_and_check(msg_size);
}


void
VC5Application::build_msg_set_color(const uint8_t led_index, const ColorTemplate& color)
{
	unique_id_++;

	VC5_MsgHeader* header = reinterpret_cast<VC5_MsgHeader*>(cmd_buffer_.data());
	header->message_length = sizeof(VC5_MsgHeader) + sizeof(VC5_SetLed);
	header->message_code = VC5_MsgHeader::cmd_set_led;
	header->unique_id = unique_id_;
	header->message_status = 0;
	header->reserved = 0;

	VC5_SetLed* data = reinterpret_cast<VC5_SetLed*>(cmd_buffer_.data() + sizeof(VC5_MsgHeader));
	data->led_index = led_index;
	data->r = color.r;
	data->g = color.g;
	data->b = color.b;
}


WinError
VC5Application::build_msg_print_string(uint8_t display_index, const char* string, uint8_t padding_top, uint8_t padding_left)
{
	unique_id_++;

	// total message size for Font12 is 30 bytes
	VC5_MsgHeader* header = reinterpret_cast<VC5_MsgHeader*>(cmd_buffer_.data());
	header->message_length = sizeof(VC5_MsgHeader) + sizeof(VC5_PrintString);
	header->message_code = VC5_MsgHeader::cmd_print_string;
	header->unique_id = unique_id_;
	header->message_status = 0;
	header->reserved = 0;

	VC5_PrintString* data = reinterpret_cast<VC5_PrintString*>(cmd_buffer_.data() + sizeof(VC5_MsgHeader));
	data->display_index = display_index;
	data->padding_top_px = padding_top;
	data->padding_left_px = padding_left;
	errno_t succ = strcpy_s(data->string, VC5_PrintString::max_character, string);
	if (succ)
	{
		// unexpected error
		std::cout << std::format("{}: could not create msg\n", __func__);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	return NO_ERROR;
}


WinError 
VC5Application::transmit_and_check(uint msg_size)
{
	// write pipe
	WinError err = interface1_.write_pipe_sync(EPNUM_VENDOR_1_BULK_OUT, cmd_buffer_.data(), msg_size);
	if (err != NO_ERROR)
	{
		return err;
	}

	// read pipe
	size_t len_transferred = 0;
	err = interface1_.read_pipe_sync(EPNUM_VENDOR_1_BULK_IN, rsp_buffer_.data(), sizeof(rsp_buffer_), len_transferred);
	if (err != NO_ERROR)
	{
		return err;
	}

	if (len_transferred == 0)
	{
		// read timed out
		std::cout << std::format("Read response time out.");
		return ERROR_TIMEOUT;
	}

	// process response
	err = process_rsp(len_transferred);
	if (err != NO_ERROR)
	{
		return err;
	}

	return err;
}


WinError 
VC5Application::process_rsp(size_t len_transferred)
{
	// response always contains only the MsgHeader
	if (len_transferred != sizeof(VC5_MsgHeader))
	{
		// unexpected length
		std::cout << std::format("Vendor 1: unexpected length = {}\n", len_transferred);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	const VC5_MsgHeader* rsp = reinterpret_cast<VC5_MsgHeader*> (rsp_buffer_.data());

	if (rsp->message_length != sizeof(VC5_MsgHeader))
	{
		// unexpected rsp length
		std::cout << std::format("Vendor 1: unexpected rsp length = {}\n", rsp->message_length);
		return ERROR_ERRORS_ENCOUNTERED;
	}
	if (rsp->message_code != VC5_MsgHeader::rsp_done)
	{
		// unexpected msg
		std::cout << std::format("Vendor 1: expected a response, got {} instead\n", rsp->message_code);
		return ERROR_ERRORS_ENCOUNTERED;
	}
	if (rsp->unique_id != unique_id_)
	{
		// unexpected unique_id
		std::cout << std::format("Vendor 1: unexpected unique_id, got {}\n", rsp->unique_id);
		return ERROR_ERRORS_ENCOUNTERED;
	}
	if (rsp->message_status != VC5_MsgHeader::st_success)
	{
		// unexpected error
		std::cout << std::format("Vendor 1: VC5 returned an error = {}\n", rsp->message_status);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	// success
	//std::cout << std::format("Vendor 1: command executed successfully\n");

	return NO_ERROR;
}


WinError 
VC5Application::print_vol_value(const VC5Unit& unit)
{
	using namespace std::string_literals;

	unsigned int volume = unit.get_volume();

	// convert int to string
	std::string s = std::to_string(volume) + " %"s;

	// resize string and move \0 to the end, "100 %" = 5 characters + \0
	s.resize(6, ' ');
	char& back = s.back();
	back = '\0';

	// copy the new string into a char[] buffer
	char string[VC5_PrintString::max_character];
	errno_t succ = strcpy_s(string, (VC5_PrintString::max_character), s.c_str());
	if (succ)
	{
		// strcpy_s failed
		std::cout << std::format("{}: strcpy_s failed\n", __func__);
		return ERROR_ERRORS_ENCOUNTERED;
	}

	return print_string_on_display(unit.get_display_index(), string, disp_lines::volume_line_num, disp_lines::volume_padding_left);
}


