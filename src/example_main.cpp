/* *****************************************************************************
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created: 03/23/10 01:31:15
 *  Copyright (c) 2010 Perry L. Hung. All rights reserved.
 *
 * ****************************************************************************/

/**
 *  @file example_main.cpp
 *
 *  @brief Sample main.cpp file. Blinks an LED, sends a message out USART2 and
 *  the USB virtual COM port, and turns on PWM on pin 2
 */

#ifndef _EXAMPLE_MAIN_H_
#define _EXAMPLE_MAIN_H_

#endif
#include "wiring.h"
#include "HardwareSerial.h"
#include "HardwareUsb.h"
#include "math.h"
#include "usb.h"

uint8_t bytes_in;

#define LED_PIN 13
#define PWM_PIN  2

HardwareUsb Usb;

void setup()
{
    /* Set up the LED to blink  */
    pinMode(LED_PIN, OUTPUT);

    /* Send a message out USART2  */
    Serial2.begin(9600);
    Serial2.println("Hello world!");

    /* Turn on PWM on pin PWM_PIN */
    pinMode(PWM_PIN, PWM);
    pwmWrite(PWM_PIN, 0x8000);

    /* Send a message out the USB virtual com port  */
    Usb.println("Hello world!");
}

int toggle = 0;

void loop() {
    toggle ^= 1;
    digitalWrite(LED_PIN, toggle);
    delay(100);
}


int main(void) {
    init();
    setup();

    while (1) {
        loop();
    }
    return 0;
}

/* Required for C++ hackery */
/* TODO: This really shouldn't go here... move it later
 * */
extern "C" void __cxa_pure_virtual(void) {
    while(1)
        ;
}