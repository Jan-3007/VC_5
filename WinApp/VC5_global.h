#pragma once


// C++
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <array>


// Microsoft
#include <conio.h>


// Windows
// exclude GUI stuff to speed up compilation
#define WIN32_LEAN_AND_MEAN	1
#include <Windows.h>
#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")


// WinUSB
#include <winusb.h>
#pragma comment(lib, "winusb.lib")


// VC5
#include "WinUtils.h"
#include "ColorTemplate.h"
#include "VC5_usb_protocol.h"
#include "VC5_app_config.h"

#include "WinUSBDevice.h"
#include "WinUSBInterface.h"

#include "VC5Unit.h"
#include "VC5Application.h"
