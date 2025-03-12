#pragma once



//--------------------
// C++
//--------------------
#include <stdio.h>
#include <cassert>
#include <array>



//--------------------
// Pico SDK
//--------------------
#include "pico/stdlib.h"
#include "RP2040.h"
#include "hardware/i2c.h"
#include "pico/unique_id.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"


#if LIB_PICO_MULTICORE
    #error dont want to support multicore
#endif



//--------------------
// FreeRTOS
//--------------------
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"


// check consistency with __NVIC_PRIO_BITS given in FreeRTOSConfig.h
#if (__NVIC_PRIO_BITS != 2)
    #error unexpected value for __NVIC_PRIO_BITS
#endif



//--------------------
// TinyUSB
//--------------------
#include "tusb.h"
#include "tusb_config.h"
#include "usb_descriptors.h"


//--------------------
// Waveshare
//--------------------
extern "C"
{
#include "paint/GUI/GUI_Paint.h"
}



//--------------------
// Custom
//--------------------
#include "VC5_config.h"
#include "FreeRTOS_support.h"
#include "Mutex.h"
#include "VC5_RP2040.h"
#include "VC5_usb_protocol.h"

#include "MCP23017.h"
#include "rotary.h"
#include "rotary_bank.h" 

#include "led_WS2812_pio_init.h"
#include "led_WS2812.h"

#include "display_driver_itf.h"
#include "SH1106_disp_driver.h"
#include "VC5_display_row.h"

#include "usb_cmd_processor.h"
#include "usb_device.h"




#ifdef __cplusplus
extern "C"
#endif
void fatal_error(int reason);



