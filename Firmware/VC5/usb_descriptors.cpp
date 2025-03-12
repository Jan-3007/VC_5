/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "VC5_global.h"


#define VC5_DEVICE_BCD   0x0100     //firmware version


// string descriptors
enum
{
    STR_IDX_MANUFACTURER = 1,
    STR_IDX_PRODUCT,
    STR_IDX_SERIAL_NUMBER,
    STR_IDX_VENDOR_IAD,
    STR_IDX_VENDOR_1_INTERFACE,
    STR_IDX_VENDOR_2_INTERFACE,
};


//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
const tusb_desc_device_t desc_device =
{
  .bLength            = sizeof(tusb_desc_device_t),
  .bDescriptorType    = TUSB_DESC_DEVICE,
  .bcdUSB             = 0x0210, // at least 2.1 for BOS

  .bDeviceClass       = 0x00,
  .bDeviceSubClass    = 0x00,
  .bDeviceProtocol    = 0x00,
  .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

  .idVendor           = VC5_USB_VID,
  .idProduct          = VC5_USB_PID,
  .bcdDevice          = VC5_DEVICE_BCD,

  .iManufacturer      = STR_IDX_MANUFACTURER,
  .iProduct           = STR_IDX_PRODUCT,
  .iSerialNumber      = STR_IDX_SERIAL_NUMBER,

  .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
extern "C"
const uint8_t* tud_descriptor_device_cb()
{
  return (uint8_t const*) &desc_device;
}


//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

#define VENDOR_INTERRUPT_EP_INTERVAL        4   // ms
#define VENDOR_IAD_LEN                      8
#define VENDOR_1_DESC_LEN                   (9 + 7 + 7)
#define VENDOR_2_DESC_LEN                   (9 + 7)


#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + VENDOR_IAD_LEN + VENDOR_1_DESC_LEN + VENDOR_2_DESC_LEN)

const uint8_t desc_configuration[] =
{
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0, 100),

    /* Interface Association */
    8, TUSB_DESC_INTERFACE_ASSOCIATION, ITF_NUM_VENDOR_1, 2, TUSB_CLASS_VENDOR_SPECIFIC, 0x00, 0x00, STR_IDX_VENDOR_IAD,

    // Vendor specific interface 1
    /* Interface */
    9, TUSB_DESC_INTERFACE, ITF_NUM_VENDOR_1, 0, 2, TUSB_CLASS_VENDOR_SPECIFIC, 0x00, 0x00, STR_IDX_VENDOR_1_INTERFACE,
    /* Endpoint Bulk Out, for command messages */ 
    7, TUSB_DESC_ENDPOINT, EPNUM_VENDOR_1_BULK_OUT, TUSB_XFER_BULK, U16_TO_U8S_LE(VENDOR_BULK_EP_SIZE), 0,
    /* Endpoint Bulk In, for response messages */
    7, TUSB_DESC_ENDPOINT, EPNUM_VENDOR_1_BULK_IN, TUSB_XFER_BULK, U16_TO_U8S_LE(VENDOR_BULK_EP_SIZE), 0,

    // Vendor specific interface 2
    /* Interface */
    9, TUSB_DESC_INTERFACE, ITF_NUM_VENDOR_2, 0, 1, TUSB_CLASS_VENDOR_SPECIFIC, 0x00, 0x00, STR_IDX_VENDOR_2_INTERFACE,
    /* Endpoint Interrupt In, for event messages */
    7, TUSB_DESC_ENDPOINT, EPNUM_VENDOR_2_INT_IN, TUSB_XFER_INTERRUPT, U16_TO_U8S_LE(VENDOR_INTERRUPT_EP_SIZE), VENDOR_INTERRUPT_EP_INTERVAL,
};


// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
extern "C"
const uint8_t* tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations

  return desc_configuration;
}


//--------------------------------------------------------------------+
// BOS Descriptor
//--------------------------------------------------------------------+

/* Microsoft OS 2.0 registry property descriptor
Per MS requirements https://msdn.microsoft.com/en-us/library/windows/hardware/hh450799(v=vs.85).aspx
device should create DeviceInterfaceGUIDs. It can be done by driver and
in case of real PnP solution device should expose MS "Microsoft OS 2.0
registry property descriptor". Such descriptor can insert any record
into Windows registry per device/configuration/interface. In our case it
will insert "DeviceInterfaceGUIDs" multistring property.

GUID is freshly generated and should be OK to use.

https://developers.google.com/web/fundamentals/native-hardware/build-for-webusb/
(Section Microsoft OS compatibility descriptors)
*/

#define BOS_TOTAL_LEN               (TUD_BOS_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

#define MS_OS_20_DESC_LEN           0xB2
#define MS_OS_20_DESCIPTOR_INDEX    7

// BOS Descriptor is required for MS OS 2.0
static const uint8_t desc_bos[] =
{
  // total length, number of device caps
  TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 1),

  // Microsoft OS 2.0 descriptor
  TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)
};

extern "C"
const uint8_t* tud_descriptor_bos_cb(void)
{
  return desc_bos;
}


//--------------------------------------------------------------------+
// MS OS 2.0 descriptor
//--------------------------------------------------------------------+

static const uint8_t desc_ms_os_20[] =
{
    // Set header: length, type, windows version, total length
    U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

    // Configuration subset header: length, type, configuration index, reserved, configuration total length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A),

    // Function Subset header: length, type, first interface, reserved, subset length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), ITF_NUM_VENDOR_1, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08),

    // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub compatible ID
    U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sub-compatible

    // MS OS 2.0 Registry property descriptor: length, type
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A-0x08-0x08-0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
    U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
    'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
    U16_TO_U8S_LE(0x0050), // wPropertyDataLength
    //bPropertyData: VC5_WINUSB_INTERFACE_GUID_STR == {EC03545C-4179-4324-B2B2-24E4A976F2E7}
    '{', 0x00, 'E', 0x00, 'C', 0x00, '0', 0x00, '3', 0x00, '5', 0x00, '4', 0x00, '5', 0x00, 'C', 0x00, '-', 0x00,
    '4', 0x00, '1', 0x00, '7', 0x00, '9', 0x00, '-', 0x00, '4', 0x00, '3', 0x00, '2', 0x00, '4', 0x00, '-', 0x00,
    'B', 0x00, '2', 0x00, 'B', 0x00, '2', 0x00, '-', 0x00, '2', 0x00, '4', 0x00, 'E', 0x00, '4', 0x00, 'A', 0x00,
    '9', 0x00, '7', 0x00, '6', 0x00, 'F', 0x00, '2', 0x00, 'E', 0x00, '7', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
};

static_assert(sizeof(desc_ms_os_20) == MS_OS_20_DESC_LEN, "Incorrect size");

bool 
get_ms_os_desc_request(uint8_t rhport, const tusb_control_request_t* request)
{
    if (request->wIndex == MS_OS_20_DESCIPTOR_INDEX)
    {
        // Get Microsoft OS 2.0 compatible descriptor
        uint16_t total_len = desc_ms_os_20[8] + (static_cast<uint16_t>(desc_ms_os_20[9]) << 8);

        return tud_control_xfer(rhport, request, const_cast<uint8_t*>(desc_ms_os_20), total_len);
    }
    return false;
}


//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
extern "C"
const uint16_t* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;

    // buffer that holds the final string descriptor
    constexpr unsigned int desc_str_max_len = 32;
    static uint16_t _desc_str[desc_str_max_len];
    // buffer to hold flash ID
    static char _serial_str[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];


    const char* str;


    switch(index)
    {
        // langid
        case 0:
            _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 + 2);
            _desc_str[1] = 0x0409;   // English US
            return _desc_str;

        case STR_IDX_SERIAL_NUMBER:
            pico_get_unique_board_id_string(_serial_str, sizeof(_serial_str));
            str = _serial_str;
            break;

        case STR_IDX_MANUFACTURER:
            str = "Custom";
            break;

        case STR_IDX_PRODUCT:
        case STR_IDX_VENDOR_IAD:
            str = "VC5";
            break;
            
        case STR_IDX_VENDOR_1_INTERFACE:
            str = "Command Interface";
            break;

        case STR_IDX_VENDOR_2_INTERFACE:
            str = "Event Interface";
            break;

        default:
            // unknown index
            return NULL;
    } // switch

    unsigned int chr_count = strlen(str);

    // Convert ASCII string into UTF-16
    unsigned int i = 0;
    for(; i < chr_count && i < (desc_str_max_len - 1); i++)
    {
        // convert char to wchar
        _desc_str[i + 1] = str[i];
    }

    // first byte is length (including header), second byte is string type
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 + 2 * i);

    return _desc_str;
}


