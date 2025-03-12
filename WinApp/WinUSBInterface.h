#pragma once



class WinUSBInterface
{
protected:
	WINUSB_INTERFACE_HANDLE itf_handle_{ INVALID_HANDLE_VALUE };

public:
	// ctor
	WinUSBInterface();
	// dtor
	~WinUSBInterface();

	// check if handle is valid
	bool
	is_valid() const;

	// get interface handle
	WINUSB_INTERFACE_HANDLE
	get_itf_handle() const
	{
		return itf_handle_;
	}

	// initialise WinUSB, get an interface handle
	WinError
	init(const HANDLE device_handle);

	// get associated interface
	WinError
	get_associated_interface(const WINUSB_INTERFACE_HANDLE itf_handle, uint8_t itf_index);

	// free WinUSB interface handle, if valid
	WinError
	free();

	WinError
	flush_pipe(uint8_t pipe_id);

	WinError
	reset_pipe(uint8_t pipe_id);

	// read the interface pipe
	WinError
	read_pipe_sync(uint8_t pipe_id, void* buffer, size_t buf_len, size_t& len_transferred);

	// write to the interface pipe
	WinError 
	write_pipe_sync(uint8_t pipe_id, void* buffer, size_t buf_len, size_t* len_transferred = nullptr);

	WinError
	set_pipe_autoclear_stall(uint8_t pipe_id, uint8_t val);

	WinError
	set_pipe_policy(uint8_t pipe_id, uint32_t policy_type, void* val, uint32_t len);

	WinError
	set_pipe_transfer_timeout(uint8_t pipe_id, uint32_t millisecs);

};

