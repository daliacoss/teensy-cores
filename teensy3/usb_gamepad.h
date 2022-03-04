/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef USBgamepad_h_
#define USBgamepad_h_

#include "usb_desc.h"

#if defined(GAMEPAD_INTERFACE)

#include <inttypes.h>

#define USB_GAMEPAD_DATA_LEN (GAMEPAD_SIZE+3)/4
#define GAMEPAD_DIR_UP 0
#define GAMEPAD_DIR_UP_RIGHT 1
#define GAMEPAD_DIR_RIGHT 2
#define GAMEPAD_DIR_DOWN_RIGHT 3
#define GAMEPAD_DIR_DOWN 4
#define GAMEPAD_DIR_DOWN_LEFT 5
#define GAMEPAD_DIR_LEFT 6
#define GAMEPAD_DIR_UP_LEFT 7
#define GAMEPAD_DIR_CENTER 8
#define GAMEPAD_NUM_BUTTONS 12

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_gamepad_send(void);
extern uint32_t usb_gamepad_data[USB_GAMEPAD_DATA_LEN];
extern uint8_t gamepad_report_desc[];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_gamepad_class
{
    public:
        void begin(void) { }
        void end(void) { }

        void button(uint8_t button, bool val) {
            if (button >= GAMEPAD_NUM_BUTTONS) return;
            if (val) {
                usb_gamepad_data[0] |= (1 << button);
            }
            else {
                usb_gamepad_data[0] &= ~(1 << button);
            }
            //usb_gamepad_data[0] = 0xffffffff;
        }
        void axis(uint8_t axis, float val){

            uint32_t val_i;
            switch(axis) {
                case 0:
                    val_i = val * 0xFFFF;
                    usb_gamepad_data[0] = (usb_gamepad_data[0] & 0x0000FFFF) | (val_i << 16);
                    break;
                case 1:
                    val_i = val * 0xFFFF;
                    usb_gamepad_data[1] = (usb_gamepad_data[1] & 0xFFFF0000) | val_i;
                    break;
                case 2:
                    val_i = val * 0xFF;
                    usb_gamepad_data[1] = (usb_gamepad_data[1] & 0xFF00FFFF) | (val_i << 16);
                    break;
                case 3:
                    val_i = val * 0xFFFF;
                    usb_gamepad_data[1] = (usb_gamepad_data[1] & 0x00FFFFFF) | (val_i << 24);
                    usb_gamepad_data[2] = (usb_gamepad_data[2] & 0xFFFFFF00) | val_i >> 8;
                    break;
                case 4:
                    val_i = val * 0xFFFF;
                    usb_gamepad_data[2] = (usb_gamepad_data[2] & 0xFF0000FF) | (val_i << 8);
                    break;
                case 5:
                    val_i = val * 0xFF;
                    usb_gamepad_data[2] = (usb_gamepad_data[2] & 0x00FFFFFF) | (val_i << 24);
                    break;
            }
        }
        inline void hat(uint8_t val) {
            if (val >= 8){
                val = 15;
            }
            usb_gamepad_data[0] = (usb_gamepad_data[0] & 0xFFFF0FFF) | (val << 12);
        }

        int send_now(void) {
            return usb_gamepad_send();
        }

        void set_report_desc_button(uint8_t button, uint8_t code){
            if (button >= GAMEPAD_NUM_BUTTONS) {
                return;
            }
            gamepad_report_desc[17 + (button * 2)] = code;
        }
};
extern usb_gamepad_class Gamepad;

#endif // __cplusplus

#endif // GAMEPAD_INTERFACE

#endif // USBgamepad_h_

