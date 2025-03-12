#pragma once

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
 */

enum
{
    ITF_NUM_VENDOR_1 = 0,
    ITF_NUM_VENDOR_2,
    ITF_NUM_TOTAL
};

// tud_vendor_n_write_available actually requires an index (0 based)
#define ITF_INDEX_VENDOR_1  0
#define ITF_INDEX_VENDOR_2  1


// max packet size for endpoints
#define VENDOR_BULK_EP_SIZE                 64  // bytes
#define VENDOR_INTERRUPT_EP_SIZE            4   // bytes



// for MS OS 2.0 desc
enum
{
  VENDOR_REQUEST_MICROSOFT = 42
};


bool 
get_ms_os_desc_request(uint8_t rhport, const tusb_control_request_t* request);

