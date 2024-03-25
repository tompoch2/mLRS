//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP RxClock
//********************************************************
#ifndef ESP_RXCLOCK_H
#define ESP_RXCLOCK_H
#pragma once


#define USING_TIM_DIV1 true

#if defined(ESP8266)
  #include "ESP8266TimerInterrupt.h"
  ESP8266Timer ITimer;
#elif defined(ESP32) 
  #include "ESP32TimerInterrupt.h"
  ESP32Timer ITimer(1);
#endif
// we use a 10us time base, so that overrun is 655 ms
// 65 ms would only be 3 packets
// I have tested it with 1us time base, and it also works fine, but hey
// Note that TIM2 may be 16 bit or 32 bit depending on which STM32 controller is used


#define CLOCK_SHIFT_10US          100 // 75 // 100 // 1 ms

volatile bool doPostReceive;

uint16_t CLOCK_PERIOD_10US; // does not change while isr is enabled, so no need for volatile

volatile uint32_t CNT_10us = 0;
volatile uint32_t CCR1 = 0;
volatile uint32_t CCR3 = 0;
volatile uint32_t MS_C = 0;

//-------------------------------------------------------
// Clock ISR
//-------------------------------------------------------

IRQHANDLER(
#if defined(ESP8266)
IRAM_ATTR void CLOCK_IRQHandler(void)
#elif defined(ESP32)
IRAM_ATTR bool CLOCK_IRQHandler(void * timerNo)
#endif
{
    CNT_10us++;

    // Call HAL_IncTick every 1ms
    if (CNT_10us == MS_C) {
        MS_C = CNT_10us + 100; 
        HAL_IncTick();
    }

    // this is at about when RX was or was supposed to be received
    if (CNT_10us == CCR1) { 
        CCR3 = CNT_10us + CLOCK_SHIFT_10US; // next doPostReceive
        CCR1 = CNT_10us + CLOCK_PERIOD_10US; // next tick
    }

    // this is 1 ms after RX was or was supposed to be received
    if (CNT_10us == CCR3) { 
        doPostReceive = true;
    }

    #if defined(ESP32)
    return true;
    #endif

})


//-------------------------------------------------------
// RxClock Class
//-------------------------------------------------------

class RxClockBase
{
  public:
    void Init(uint16_t period_ms);
    void SetPeriod(uint16_t period_ms);
    void Reset(void);
};


void RxClockBase::Init(uint16_t period_ms)
{
    CLOCK_PERIOD_10US = period_ms * 100; // frame rate in units of 10us
    doPostReceive = false;
    ITimer.attachInterruptInterval(10, CLOCK_IRQHandler);
    Reset();
}


void RxClockBase::SetPeriod(uint16_t period_ms)
{
    CLOCK_PERIOD_10US = period_ms * 100;
}


void RxClockBase::Reset(void)
{
    if (!CLOCK_PERIOD_10US) while (1) {}

    __disable_irq();
    CCR1 = CNT_10us + CLOCK_PERIOD_10US;
    CCR3 = CNT_10us + CLOCK_SHIFT_10US;
    MS_C = CNT_10us + 100;
    __enable_irq();
}

#endif // ESP_RXCLOCK_H
