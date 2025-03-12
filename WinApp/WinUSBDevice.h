#pragma once



class WinUSBDevice
{
protected:
	HANDLE device_handle_{ INVALID_HANDLE_VALUE };

public:
	// ctor
	WinUSBDevice();
	// dtor
	~WinUSBDevice();

	// check if handle is valid
	bool
	is_valid() const;

	// get interface handle
	HANDLE
	get_device_handle() const
		{
			return device_handle_;
		}

	// open device handle
	WinError
	open(const WCHAR* device_path);

	// close device handle
	WinError
	close();

};

WinError
enumerate_devices(
	const GUID& device_itf_guid, 
	std::vector<std::wstring>& device_path_list
	);

