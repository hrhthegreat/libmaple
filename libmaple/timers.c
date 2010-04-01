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
 *  Created: 12/18/09 02:37:54
 *  Copyright (c) 2009 Perry L. Hung. All rights reserved.
 *
 * ****************************************************************************/

/**
 *  @file timers.c
 *
 *  @brief General timer routines
 */

#include "libmaple.h"
#include "rcc.h"
#include "timers.h"

typedef struct {
    volatile uint16 CR1;
    uint16  RESERVED0;
    volatile uint16 CR2;
    uint16  RESERVED1;
    volatile uint16 SMCR;
    uint16  RESERVED2;
    volatile uint16 DIER;
    uint16  RESERVED3;
    volatile uint16 SR;
    uint16  RESERVED4;
    volatile uint16 EGR;
    uint16  RESERVED5;
    volatile uint16 CCMR1;
    uint16  RESERVED6;
    volatile uint16 CCMR2;
    uint16  RESERVED7;
    volatile uint16 CCER;
    uint16  RESERVED8;
    volatile uint16 CNT;
    uint16  RESERVED9;
    volatile uint16 PSC;
    uint16  RESERVED10;
    volatile uint16 ARR;
    uint16  RESERVED11;
    volatile uint16 RCR;
    uint16  RESERVED12;
    volatile uint16 CCR1;
    uint16  RESERVED13;
    volatile uint16 CCR2;
    uint16  RESERVED14;
    volatile uint16 CCR3;
    uint16  RESERVED15;
    volatile uint16 CCR4;
    uint16  RESERVED16;
    volatile uint16 BDTR;   // Not used in general purpose timers
    uint16  RESERVED17;     // Not used in general purpose timers
    volatile uint16 DCR;
    uint16  RESERVED18;
    volatile uint16 DMAR;
    uint16  RESERVED19;
} Timer;

void timer_init(uint8 timer_num, uint16 prescale) {
    Timer *timer;
    uint32 is_advanced = 0;

    ASSERT(timer_num > 0 && timer_num <= 4);

    switch(timer_num) {
    case 1:
        timer = (Timer*)TIMER1_BASE;
        rcc_enable_clk_timer1();
        is_advanced = 1;
        break;
    case 2:
        timer = (Timer*)TIMER2_BASE;
        rcc_enable_clk_timer2();
        break;
    case 3:
        timer = (Timer*)TIMER3_BASE;
        rcc_enable_clk_timer3();
        break;
    case 4:
        timer = (Timer*)TIMER4_BASE;
        rcc_enable_clk_timer4();
        break;
    }

    timer->CR1 = ARPE;        // No clock division
                              // Do not buffer auto-reload preload
                              // Edge aligned
                              // Upcounter
                              // Do not stop counter at update event
                              // Update events enabled (etc, see bits [1:2])
                              // Counter disabled for now

    timer->PSC = prescale;    // Prescaling by prescale (duh)
    timer->ARR = 0xFFFF;      // Max reload cont

    /* initialize all the channels to 50% duty cycle,
     * TODO: none of them actually get output unless the gpio pin
     * is set, this will probably consume a bit more power but
     * we'll worry about that later. */
    timer->CCR1   = 0x8FFF;     // PWM start value
    timer->CCMR1 |= 0x68;       // PWM mode 1, enable preload register.
    timer->CCER  |= 0x001;      // enable ch

    timer->CCR2   = 0x8FFF;     // PWM start value
    timer->CCMR1 |= (0x68 << 8);// PWM mode 1, enable preload register.
    timer->CCER  |= 0x010;      // enable ch

    timer->CCR3   = 0x8FFF;     // PWM start value
    timer->CCMR2 |= 0x68;       // PWM mode 1, enable preload register.
    timer->CCER  |= 0x100;      // enable ch

    timer->CCR4   = 0x8FFF;     // PWM start value
    timer->CCMR2 |= (0x68 << 8);// PWM mode 1, enable preload register.
    timer->CCER  |= 0x1000;      // enable ch

    /* Advanced timer?  */
    if (is_advanced) {
        timer->BDTR = 0x8000;     // moe enable
    }

    timer->DIER = 0;          // disable update interrupt
    timer->EGR  = 1;          // Initialize update event and shadow registers
    timer->CR1  |= 1;         // Enable timer
}

void timers_disable(void) {
    Timer *timer;
    int i;
    Timer *timers[4] = {
        (Timer*)TIMER1_BASE,
        (Timer*)TIMER2_BASE,
        (Timer*)TIMER3_BASE,
        (Timer*)TIMER4_BASE,
    };

    for (i = 0; i < 4; i++) {
        timer = timers[i];
        timer->CR1 = 0;
        timer->CCER = 0;
    }
}

void timers_disable_channel(uint8 timer_num, uint8 channel) {
    Timer *timer;
    switch (timer_num) {
    case 1:
        timer = (Timer*)TIMER1_BASE;
        break;
    case 2:
        timer = (Timer*)TIMER2_BASE;
        break;
    case 3:
        timer = (Timer*)TIMER3_BASE;
        break;
    case 4:
        timer = (Timer*)TIMER4_BASE;
        break;
    default:
        ASSERT(0);
    }

    switch (channel) {
    case 1:
        timer->CCER &= ~(0x1);
        break;
    case 2:
        timer->CCER &= ~(0x10);
        break;
    case 3:
        timer->CCER &= ~(0x100);
        break;
    case 4:
        timer->CCER &= ~(0x1000);
        break;
    default:
        ASSERT(0);
    }
}