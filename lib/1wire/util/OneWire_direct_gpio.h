/*
Copyright (c) 2007, Jim Studt  (original old version - many contributors since)

The latest version of this library may be found at:
  http://www.pjrc.com/teensy/td_libs_OneWire.html

OneWire has been maintained by Paul Stoffregen (paul@pjrc.com) since
January 2010.

DO NOT EMAIL for technical support, especially not for ESP chips!
All project support questions must be posted on public forums
relevant to the board or chips used.  If using Arduino, post on
Arduino's forum.  If using ESP, post on the ESP community forums.
There is ABSOLUTELY NO TECH SUPPORT BY PRIVATE EMAIL!

Github's issue tracker for OneWire should be used only to report
specific bugs.  DO NOT request project support via Github.  All
project and tech support questions must be posted on forums, not
github issues.  If you experience a problem and you are not
absolutely sure it's an issue with the library, ask on a forum
first.  Only use github to report issues after experts have
confirmed the issue is with OneWire rather than your project.

Back in 2010, OneWire was in need of many bug fixes, but had
been abandoned the original author (Jim Studt).  None of the known
contributors were interested in maintaining OneWire.  Paul typically
works on OneWire every 6 to 12 months.  Patches usually wait that
long.  If anyone is interested in more actively maintaining OneWire,
please contact Paul (this is pretty much the only reason to use
private email about OneWire).

OneWire is now very mature code.  No changes other than adding
definitions for newer hardware support are anticipated.

Version 2.3:
  Unknown chip fallback mode, Roger Clark
  Teensy-LC compatibility, Paul Stoffregen
  Search bug fix, Love Nystrom

Version 2.2:
  Teensy 3.0 compatibility, Paul Stoffregen, paul@pjrc.com
  Arduino Due compatibility, http://arduino.cc/forum/index.php?topic=141030
  Fix DS18B20 example negative temperature
  Fix DS18B20 example's low res modes, Ken Butcher
  Improve reset timing, Mark Tillotson
  Add const qualifiers, Bertrik Sikken
  Add initial value input to crc16, Bertrik Sikken
  Add target_search() function, Scott Roberts

Version 2.1:
  Arduino 1.0 compatibility, Paul Stoffregen
  Improve temperature example, Paul Stoffregen
  DS250x_PROM example, Guillermo Lovato
  PIC32 (chipKit) compatibility, Jason Dangel, dangel.jason AT gmail.com
  Improvements from Glenn Trewitt:
  - crc16() now works
  - check_crc16() does all of calculation/checking work.
  - Added read_bytes() and write_bytes(), to reduce tedious loops.
  - Added ds2408 example.
  Delete very old, out-of-date readme file (info is here)

Version 2.0: Modifications by Paul Stoffregen, January 2010:
http://www.pjrc.com/teensy/td_libs_OneWire.html
  Search fix from Robin James
    http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27
  Use direct optimized I/O in all cases
  Disable interrupts during timing critical sections
    (this solves many random communication errors)
  Disable interrupts during read-modify-write I/O
  Reduce RAM consumption by eliminating unnecessary
    variables and trimming many to 8 bits
  Optimize both crc8 - table version moved to flash

Modified to work with larger numbers of devices - avoids loop.
Tested in Arduino 11 alpha with 12 sensors.
26 Sept 2008 -- Robin James
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27

Updated to work with arduino-0008 and to include skip() as of
2007/07/06. --RJL20

Modified to calculate the 8-bit CRC directly, avoiding the need for
the 256-byte lookup table to be loaded in RAM.  Tested in arduino-0010
-- Tom Pollard, Jan 23, 2008

Jim Studt's original library was modified by Josh Larios.

Tom Pollard, pollard@alum.mit.edu, contributed around May 20, 2008

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Much of the code was inspired by Derek Yerger's code, though I don't
think much of that remains.  In any event that was..
    (copyleft) 2006 by Derek Yerger - Free to distribute freely.

The CRC code was excerpted and inspired by the Dallas Semiconductor
sample code bearing this copyright.
//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Dallas Semiconductor
// shall not be used except as stated in the Dallas Semiconductor
// Branding Policy.
//--------------------------------------------------------------------------
*/
#ifndef OneWire_Direct_GPIO_h
#define OneWire_Direct_GPIO_h

// This header should ONLY be included by OneWire.cpp.  These defines are
// meant to be private, used within OneWire.cpp, but not exposed to Arduino
// sketches or other libraries which may include OneWire.h.

#include <stdint.h>

// Platform specific I/O definitions

#if defined(__AVR__)
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR asm("r30")
#define IO_REG_MASK_ATTR
#if defined(__AVR_ATmega4809__)
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)-8)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)-8)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)-4)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)-4)) |= (mask))
#else
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))
#endif

#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK66FX1M0__) || defined(__MK64FX512__)
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (1)
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR __attribute__ ((unused))
#define DIRECT_READ(base, mask)         (*((base)+512))
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+640) = 0)
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+640) = 1)
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+256) = 1)
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+128) = 1)

#elif defined(__MKL26Z64__)
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((*((base)+16) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+20) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+20) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+8) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+4) = (mask))

#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((*((base)+2) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+1) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+1) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+34) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+33) = (mask))

#elif defined(__SAM3X8E__) || defined(__SAM3A8C__) || defined(__SAM3A4C__)
// Arduino 1.5.1 may have a bug in delayMicroseconds() on Arduino Due.
// http://arduino.cc/forum/index.php/topic,141030.msg1076268.html#msg1076268
// If you have trouble with OneWire on Arduino Due, please check the
// status of delayMicroseconds() before reporting a bug in OneWire!
#define PIN_TO_BASEREG(pin)             (&(digitalPinToPort(pin)->PIO_PER))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*((base)+15)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+5)) = (mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+4)) = (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+13)) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+12)) = (mask))
#ifndef PROGMEM
#define PROGMEM
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#endif

#elif defined(__PIC32MX__)
#define PIN_TO_BASEREG(pin)             (portModeRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*(base+4)) & (mask)) ? 1 : 0)  //PORTX + 0x10
#define DIRECT_MODE_INPUT(base, mask)   ((*(base+2)) = (mask))            //TRISXSET + 0x08
#define DIRECT_MODE_OUTPUT(base, mask)  ((*(base+1)) = (mask))            //TRISXCLR + 0x04
#define DIRECT_WRITE_LOW(base, mask)    ((*(base+8+1)) = (mask))          //LATXCLR  + 0x24
#define DIRECT_WRITE_HIGH(base, mask)   ((*(base+8+2)) = (mask))          //LATXSET + 0x28

#elif defined(ARDUINO_ARCH_ESP8266)
// Special note: I depend on the ESP community to maintain these definitions and
// submit good pull requests.  I can not answer any ESP questions or help you
// resolve any problems related to ESP chips.  Please do not contact me and please
// DO NOT CREATE GITHUB ISSUES for ESP support.  All ESP questions must be asked
// on ESP community forums.
#define PIN_TO_BASEREG(pin)             ((volatile uint32_t*) GPO)
#define PIN_TO_BITMASK(pin)             (1 << pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((GPI & (mask)) ? 1 : 0)    //GPIO_IN_ADDRESS
#define DIRECT_MODE_INPUT(base, mask)   (GPE &= ~(mask))            //GPIO_ENABLE_W1TC_ADDRESS
#define DIRECT_MODE_OUTPUT(base, mask)  (GPE |= (mask))             //GPIO_ENABLE_W1TS_ADDRESS
#define DIRECT_WRITE_LOW(base, mask)    (GPOC = (mask))             //GPIO_OUT_W1TC_ADDRESS
#define DIRECT_WRITE_HIGH(base, mask)   (GPOS = (mask))             //GPIO_OUT_W1TS_ADDRESS

#elif defined(ARDUINO_ARCH_ESP32)
#include <driver/rtc_io.h>
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(IO_REG_TYPE pin)
{
    if ( pin < 32 )
        return (GPIO.in >> pin) & 0x1;
    else if ( pin < 40 )
        return (GPIO.in1.val >> (pin - 32)) & 0x1;

    return 0;
}

static inline __attribute__((always_inline))
void directWriteLow(IO_REG_TYPE pin)
{
    if ( pin < 32 )
        GPIO.out_w1tc = ((uint32_t)1 << pin);
    else if ( pin < 34 )
        GPIO.out1_w1tc.val = ((uint32_t)1 << (pin - 32));
}

static inline __attribute__((always_inline))
void directWriteHigh(IO_REG_TYPE pin)
{
    if ( pin < 32 )
        GPIO.out_w1ts = ((uint32_t)1 << pin);
    else if ( pin < 34 )
        GPIO.out1_w1ts.val = ((uint32_t)1 << (pin - 32));
}

static inline __attribute__((always_inline))
void directModeInput(IO_REG_TYPE pin)
{
    if ( digitalPinIsValid(pin) )
    {
        uint32_t rtc_reg(rtc_gpio_desc[pin].reg);

        if ( rtc_reg ) // RTC pins PULL settings
        {
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].mux);
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].pullup | rtc_gpio_desc[pin].pulldown);
        }

        if ( pin < 32 )
            GPIO.enable_w1tc = ((uint32_t)1 << pin);
        else
            GPIO.enable1_w1tc.val = ((uint32_t)1 << (pin - 32));

        uint32_t pinFunction((uint32_t)2 << FUN_DRV_S); // what are the drivers?
        pinFunction |= FUN_IE; // input enable but required for output as well?
        pinFunction |= ((uint32_t)2 << MCU_SEL_S);

        ESP_REG(DR_REG_IO_MUX_BASE + esp32_gpioMux[pin].reg) = pinFunction;

        GPIO.pin[pin].val = 0;
    }
}

static inline __attribute__((always_inline))
void directModeOutput(IO_REG_TYPE pin)
{
    if ( digitalPinIsValid(pin) && pin <= 33 ) // pins above 33 can be only inputs
    {
        uint32_t rtc_reg(rtc_gpio_desc[pin].reg);

        if ( rtc_reg ) // RTC pins PULL settings
        {
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].mux);
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].pullup | rtc_gpio_desc[pin].pulldown);
        }

        if ( pin < 32 )
            GPIO.enable_w1ts = ((uint32_t)1 << pin);
        else // already validated to pins <= 33
            GPIO.enable1_w1ts.val = ((uint32_t)1 << (pin - 32));

        uint32_t pinFunction((uint32_t)2 << FUN_DRV_S); // what are the drivers?
        pinFunction |= FUN_IE; // input enable but required for output as well?
        pinFunction |= ((uint32_t)2 << MCU_SEL_S);

        ESP_REG(DR_REG_IO_MUX_BASE + esp32_gpioMux[pin].reg) = pinFunction;

        GPIO.pin[pin].val = 0;
    }
}

#define DIRECT_READ(base, pin)          directRead(pin)
#define DIRECT_WRITE_LOW(base, pin)     directWriteLow(pin)
#define DIRECT_WRITE_HIGH(base, pin)    directWriteHigh(pin)
#define DIRECT_MODE_INPUT(base, pin)    directModeInput(pin)
#define DIRECT_MODE_OUTPUT(base, pin)   directModeOutput(pin)
// https://github.com/PaulStoffregen/OneWire/pull/47
// https://github.com/stickbreaker/OneWire/commit/6eb7fc1c11a15b6ac8c60e5671cf36eb6829f82c
#ifdef  interrupts
#undef  interrupts
#endif
#ifdef  noInterrupts
#undef  noInterrupts
#endif
#define noInterrupts() {portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;portENTER_CRITICAL(&mux)
#define interrupts() portEXIT_CRITICAL(&mux);}
//#warning "ESP32 OneWire testing"

#elif defined(ARDUINO_ARCH_STM32)
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             ((uint32_t)digitalPinToPinName(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          digitalReadFast((PinName)pin)
#define DIRECT_WRITE_LOW(base, pin)     digitalWriteFast((PinName)pin, LOW)
#define DIRECT_WRITE_HIGH(base, pin)    digitalWriteFast((PinName)pin, HIGH)
#define DIRECT_MODE_INPUT(base, pin)    pin_function((PinName)pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0))
#define DIRECT_MODE_OUTPUT(base, pin)   pin_function((PinName)pin, STM_PIN_DATA(STM_MODE_OUTPUT_PP, GPIO_NOPULL, 0))

#elif defined(__SAMD21G18A__)
#define PIN_TO_BASEREG(pin)             portModeRegister(digitalPinToPort(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*((base)+8)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) = (mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+2)) = (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+5)) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+6)) = (mask))

#elif defined(RBL_NRF51822)
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          nrf_gpio_pin_read(pin)
#define DIRECT_WRITE_LOW(base, pin)     nrf_gpio_pin_clear(pin)
#define DIRECT_WRITE_HIGH(base, pin)    nrf_gpio_pin_set(pin)
#define DIRECT_MODE_INPUT(base, pin)    nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_NOPULL)
#define DIRECT_MODE_OUTPUT(base, pin)   nrf_gpio_cfg_output(pin)

#elif defined(__arc__) /* Arduino101/Genuino101 specifics */

#include "scss_registers.h"
#include "portable.h"
#include "avr/pgmspace.h"

#define GPIO_ID(pin)			(g_APinDescription[pin].ulGPIOId)
#define GPIO_TYPE(pin)			(g_APinDescription[pin].ulGPIOType)
#define GPIO_BASE(pin)			(g_APinDescription[pin].ulGPIOBase)
#define DIR_OFFSET_SS			0x01
#define DIR_OFFSET_SOC			0x04
#define EXT_PORT_OFFSET_SS		0x0A
#define EXT_PORT_OFFSET_SOC		0x50

/* GPIO registers base address */
#define PIN_TO_BASEREG(pin)		((volatile uint32_t *)g_APinDescription[pin].ulGPIOBase)
#define PIN_TO_BITMASK(pin)		pin
#define IO_REG_TYPE			uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    IO_REG_TYPE ret;
    if (SS_GPIO == GPIO_TYPE(pin)) {
        ret = READ_ARC_REG(((IO_REG_TYPE)base + EXT_PORT_OFFSET_SS));
    } else {
        ret = MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, EXT_PORT_OFFSET_SOC);
    }
    return ((ret >> GPIO_ID(pin)) & 0x01);
}

static inline __attribute__((always_inline))
void directModeInput(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG((((IO_REG_TYPE)base) + DIR_OFFSET_SS)) & ~(0x01 << GPIO_ID(pin)),
			((IO_REG_TYPE)(base) + DIR_OFFSET_SS));
    } else {
        MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, DIR_OFFSET_SOC) &= ~(0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directModeOutput(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(((IO_REG_TYPE)(base) + DIR_OFFSET_SS)) | (0x01 << GPIO_ID(pin)),
			((IO_REG_TYPE)(base) + DIR_OFFSET_SS));
    } else {
        MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, DIR_OFFSET_SOC) |= (0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directWriteLow(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(base) & ~(0x01 << GPIO_ID(pin)), base);
    } else {
        MMIO_REG_VAL(base) &= ~(0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directWriteHigh(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(base) | (0x01 << GPIO_ID(pin)), base);
    } else {
        MMIO_REG_VAL(base) |= (0x01 << GPIO_ID(pin));
    }
}

#define DIRECT_READ(base, pin)		directRead(base, pin)
#define DIRECT_MODE_INPUT(base, pin)	directModeInput(base, pin)
#define DIRECT_MODE_OUTPUT(base, pin)	directModeOutput(base, pin)
#define DIRECT_WRITE_LOW(base, pin)	directWriteLow(base, pin)
#define DIRECT_WRITE_HIGH(base, pin)	directWriteHigh(base, pin)

#elif defined(__riscv)

/*
 * Tested on highfive1
 *
 * Stable results are achieved operating in the
 * two high speed modes of the highfive1.  It
 * seems to be less reliable in slow mode.
 */
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             digitalPinToBitMask(pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(IO_REG_TYPE mask)
{
    return ((GPIO_REG(GPIO_INPUT_VAL) & mask) != 0) ? 1 : 0;
}

static inline __attribute__((always_inline))
void directModeInput(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_XOR)  &= ~mask;
    GPIO_REG(GPIO_IOF_EN)      &= ~mask;

    GPIO_REG(GPIO_INPUT_EN)  |=  mask;
    GPIO_REG(GPIO_OUTPUT_EN) &= ~mask;
}

static inline __attribute__((always_inline))
void directModeOutput(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_XOR)  &= ~mask;
    GPIO_REG(GPIO_IOF_EN)      &= ~mask;

    GPIO_REG(GPIO_INPUT_EN)  &= ~mask;
    GPIO_REG(GPIO_OUTPUT_EN) |=  mask;
}

static inline __attribute__((always_inline))
void directWriteLow(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~mask;
}

static inline __attribute__((always_inline))
void directWriteHigh(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_VAL) |= mask;
}

#define DIRECT_READ(base, mask)          directRead(mask)
#define DIRECT_WRITE_LOW(base, mask)     directWriteLow(mask)
#define DIRECT_WRITE_HIGH(base, mask)    directWriteHigh(mask)
#define DIRECT_MODE_INPUT(base, mask)    directModeInput(mask)
#define DIRECT_MODE_OUTPUT(base, mask)   directModeOutput(mask)

#else
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE unsigned int
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          digitalRead(pin)
#define DIRECT_WRITE_LOW(base, pin)     digitalWrite(pin, LOW)
#define DIRECT_WRITE_HIGH(base, pin)    digitalWrite(pin, HIGH)
#define DIRECT_MODE_INPUT(base, pin)    pinMode(pin,INPUT)
#define DIRECT_MODE_OUTPUT(base, pin)   pinMode(pin,OUTPUT)
#warning "OneWire. Fallback mode. Using API calls for pinMode,digitalRead and digitalWrite. Operation of this library is not guaranteed on this architecture."

#endif

#endif
