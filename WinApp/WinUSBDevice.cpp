#include "VC5_global.h"



// ctor
WinUSBDevice::WinUSBDevice()
{
	// empty
}


// dtor
WinUSBDevice::~WinUSBDevice()
{
	close();
}


WinError
WinUSBDevice::open(const WCHAR* device_path)
{
	if (is_valid())
	{
		std::cout << std::format("{}: device handle exists\n", __func__);
		return ERROR_INTERNAL_ERROR;
	}

	// open the device
	device_handle_ = ::CreateFileW(
		device_path,						//	LPCWSTR               lpFileName,
		GENERIC_READ | GENERIC_WRITE,		//	DWORD                 dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,	//	DWORD                 dwShareMode,
		nullptr,							//	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,						//	DWORD                 dwCreationDisposition,
		FILE_FLAG_OVERLAPPED,				//	DWORD                 dwFlagsAndAttributes,
		NULL								//	HANDLE                hTemplateFile
	);

	if (device_handle_ == INVALID_HANDLE_VALUE)
	{
		WinError err = ::GetLastError();
		std::cout << std::format("CreateFileW failed, err = {}\n", err);
		return err;
	}
	return NO_ERROR;
}


WinError
WinUSBDevice::close()
{
	WinError err = NO_ERROR;
	if (is_valid())
	{
		BOOL succ = ::CloseHandle(
			device_handle_			// [in] HANDLE hObject
			);
		if (!succ)
		{
			err = ::GetLastError();
			std::cout << std::format("CloseHandle failed, err = {}\n", err);
		}
		device_handle_ = INVALID_HANDLE_VALUE;
	}
	return err;
}


bool
WinUSBDevice::is_valid() const
{
	return (device_handle_ != INVALID_HANDLE_VALUE);
}




WinError
enumerate_devices(
	const GUID& device_itf_guid, 
	std::vector<std::wstring> &device_path_list
	)
{
	WinError err = NO_ERROR;

	// get device info list
	HDEVINFO device_info = ::SetupDiGetClassDevsW(
		&device_itf_guid,							// [in, optional] const GUID*	ClassGuid,
		NULL,										// [in, optional] PCWSTR		Enumerator,
		NULL,										// [in, optional] HWND			hwndParent,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE		// [in]           DWORD			Flags
		);
	if (device_info == INVALID_HANDLE_VALUE)
	{
		err = ::GetLastError();
		std::cout << std::format("SetupDiGetClassDevsW failed, err = {}\n", err);
		return err;
	}

	// iterate through device info list
	for (DWORD index = 0; ; index++)
	{
		SP_DEVICE_INTERFACE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.cbSize = sizeof(data);

		BOOL succ = ::SetupDiEnumDeviceInterfaces(
			device_info, 				//   [in]           HDEVINFO                  DeviceInfoSet,
			nullptr, 					//   [in, optional] PSP_DEVINFO_DATA          DeviceInfoData,
			&device_itf_guid, 			//   [in]           const GUID                *InterfaceClassGuid,
			index, 						//   [in]           DWORD                     MemberIndex,
			&data						//   [out]          PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData
		);
		if (!succ)
		{
			err = ::GetLastError();
			if (err == ERROR_NO_MORE_ITEMS)
			{
				// end of list
				err = NO_ERROR;
			}
			else
			{
				// true error
				std::cout << std::format("SetupDiEnumDeviceInterfaces failed, err = {}\n", err);
			}
			break;
		}

		// create detail_data with a more than enough buffer for the device path
		uint8_t detail_buffer[1024];
		SP_DEVICE_INTERFACE_DETAIL_DATA_W* detail_data = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(detail_buffer);
		detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

		succ = ::SetupDiGetDeviceInterfaceDetailW(
			device_info,				// [in] HDEVINFO                           DeviceInfoSet,
			&data,						// [in]            PSP_DEVICE_INTERFACE_DATA          DeviceInterfaceData,
			detail_data,				// [out, optional] PSP_DEVICE_INTERFACE_DETAIL_DATA_W DeviceInterfaceDetailData,
			sizeof(detail_buffer),		// [in]            DWORD                              DeviceInterfaceDetailDataSize,
			nullptr,					// [out, optional] PDWORD                             RequiredSize,
			nullptr						// [out, optional] PSP_DEVINFO_DATA                   DeviceInfoData
		);

		if (!succ)
		{
			err = ::GetLastError();
			std::cout << std::format("SetupDiGetDeviceInterfaceDetailW failed, err = {}\n", err);
			break;
		}

		// copy WCHAR string to vector as wstring 
		device_path_list.push_back(detail_data->DevicePath);
	}


	// destroy must always be executed
	::SetupDiDestroyDeviceInfoList(
		device_info						// [in] HDEVINFO DeviceInfoSet
		);
	return err;
}
