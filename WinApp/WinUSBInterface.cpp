#include "VC5_global.h"



// ctor
WinUSBInterface::WinUSBInterface()
{
	// empty
}


// dtor
WinUSBInterface::~WinUSBInterface()
{
	free();
}


bool
WinUSBInterface::is_valid() const
{
	return (itf_handle_ != INVALID_HANDLE_VALUE);
}


WinError
WinUSBInterface::init(const HANDLE device_handle)
{
	if (is_valid())
	{
		std::cout << std::format("{}: interface handle exists\n", __func__);
		return ERROR_INTERNAL_ERROR;
	}

	WinError err = NO_ERROR;
	BOOL succ = ::WinUsb_Initialize(
		device_handle,				//   [in]  HANDLE                   DeviceHandle,
		&itf_handle_				//   [out] PWINUSB_INTERFACE_HANDLE InterfaceHandle
	);
	if (!succ)
	{
		err = ::GetLastError();
		std::cout << std::format("WinUsb_Initialize failed, err = {}\n", err);
	}
	return err;
}


WinError
WinUSBInterface::get_associated_interface(const WINUSB_INTERFACE_HANDLE itf_handle, uint8_t itf_index)
{
	if (is_valid())
	{
		std::cout << std::format("{}: interface handle exists\n", __func__);
		return ERROR_INTERNAL_ERROR;
	}

	WinError err = NO_ERROR;
	BOOL succ = ::WinUsb_GetAssociatedInterface(
		itf_handle, 								//   [in]  WINUSB_INTERFACE_HANDLE  InterfaceHandle,
		itf_index, 									//   [in]  UCHAR                    AssociatedInterfaceIndex,
		&itf_handle_								//   [out] PWINUSB_INTERFACE_HANDLE AssociatedInterfaceHandle
	);
	if (!succ)
	{
		err = ::GetLastError();
		std::cout << std::format("WinUsb_GetAssociatedInterface failed, err = {}\n", err);
	}
	return err;

}


WinError
WinUSBInterface::free()
{
	WinError err = NO_ERROR;
	if (is_valid())
	{
		BOOL succ = ::WinUsb_Free(
			itf_handle_					//   [in] WINUSB_INTERFACE_HANDLE InterfaceHandle
			);
		if (!succ)
		{
			err = ::GetLastError();
			std::cout << std::format("WinUsb_Free failed, err = {}\n", err);
		}
		itf_handle_ = INVALID_HANDLE_VALUE;
	}
	return err;
}


WinError 
WinUSBInterface::flush_pipe(uint8_t pipe_id)
{
	BOOL succ = ::WinUsb_FlushPipe(
		itf_handle_,				// [in] WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id						// [in] UCHAR                   PipeID
		);
	if (!succ)
	{
		WinError err = ::GetLastError();
		std::cout << std::format("WinUsb_FlushPipe failed, err = {}\n", err);
		return err;
	}

	return NO_ERROR;
}


WinError
WinUSBInterface::reset_pipe(uint8_t pipe_id)
{
	BOOL succ = ::WinUsb_ResetPipe(
		itf_handle_,				// [in] WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id						// [in] UCHAR                   PipeID
		);
	if (!succ)
	{
		WinError err = ::GetLastError();
		std::cout << std::format("WinUsb_ResetPipe failed, err = {}\n", err);
		return err;
	}

	return NO_ERROR;
}


WinError 
WinUSBInterface::write_pipe_sync(uint8_t pipe_id, void* buffer, size_t buf_len, size_t* len_transferred)
{
	ULONG b_count = 0;

	BOOL succ = ::WinUsb_WritePipe(
		itf_handle_,					//	[in]            WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id,						//	[in]            UCHAR                   PipeID,
		static_cast<UCHAR*>(buffer),	//	[out]           PUCHAR                  Buffer,
		static_cast<ULONG>(buf_len),	//	[in]            ULONG                   BufferLength,
		&b_count,						//	[out, optional] PULONG                  LengthTransferred,
		nullptr							//	[in, optional]  LPOVERLAPPED            Overlapped
		);

	if (!succ)
	{
		WinError err = ::GetLastError();
		if (err == ERROR_SEM_TIMEOUT)
		{
			// Timeout, not an error
			b_count = 0;
		}
		else
		{
			std::cout << std::format("WinUsb_WritePipe failed, err = {}\n", err);
			return err;
		}
	}

	if (len_transferred != nullptr)
	{
		*len_transferred = b_count;
	}

	return NO_ERROR;
}



WinError
WinUSBInterface::set_pipe_autoclear_stall(uint8_t pipe_id, uint8_t val)
{
	return set_pipe_policy(pipe_id, AUTO_CLEAR_STALL, &val, sizeof(val));
}


WinError
WinUSBInterface::set_pipe_transfer_timeout(uint8_t pipe_id, uint32_t millisecs)
{
	return set_pipe_policy(pipe_id, PIPE_TRANSFER_TIMEOUT, &millisecs, sizeof(millisecs));
}


WinError
WinUSBInterface::set_pipe_policy(uint8_t pipe_id, uint32_t policy_type, void* val, uint32_t len)
{
	BOOL succ = ::WinUsb_SetPipePolicy(
		itf_handle_,			//		[in] WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id,				//		[in] UCHAR                   PipeID,
		policy_type,			//		[in] ULONG                   PolicyType,
		len,					//		[in] ULONG                   ValueLength,
		val						//		[in] PVOID                   Value
		);
	if (!succ)
	{
		WinError err = ::GetLastError();
		std::cout << std::format("WinUsb_SetPipePolicy failed, err = {}\n", err);
		return err;
	}

	return NO_ERROR;
}



WinError
WinUSBInterface::read_pipe_sync(uint8_t pipe_id, void* buffer, size_t buf_len, size_t& len_transferred)
{
	ULONG b_count = 0;

	BOOL succ = ::WinUsb_ReadPipe(
		itf_handle_,					//	[in]			WINUSB_INTERFACE_HANDLE InterfaceHandle,
		pipe_id,						//	[in]            UCHAR                   PipeID,
		static_cast<UCHAR*>(buffer),	//	[out]           PUCHAR                  Buffer,
		static_cast<ULONG>(buf_len),	//	[in]            ULONG                   BufferLength,
		&b_count,						//	[out, optional] PULONG                  LengthTransferred,
		nullptr							//	[in, optional]  LPOVERLAPPED            Overlapped
		);
	if (!succ)
	{
		WinError err = ::GetLastError();
		if (err == ERROR_SEM_TIMEOUT)
		{
			// Timeout, not an error
			b_count = 0;
		}
		else
		{
			std::cout << std::format("WinUsb_ReadPipe failed, err = {}\n", err);
			return err;
		}
	}

	len_transferred = b_count;

	return NO_ERROR;
}
