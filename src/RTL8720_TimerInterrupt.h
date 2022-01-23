/****************************************************************************************************************************
  RTL8720_TimerInterrupt.h
  For RTL8720DN, RTL8722DM and RTL8722CSM boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RTL8720_TimerInterrupt
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one RTL8720DN, RTL8722DM and RTL8722CSM timer and avoid conflicting 
  with other cores' tasks. The accuracy is nearly perfect compared to software timers. The most important feature is they're 
  ISR-based timers. Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      09/08/2021 Initial coding for RTL8720DN, RTL8722DM and RTL8722CSM
*****************************************************************************************************************************/

#pragma once

#ifndef RTL8720_TIMERINTERRUPT_H
#define RTL8720_TIMERINTERRUPT_H

#if !defined(CONFIG_PLATFORM_8721D)
  #error Only for Ameba Realtek RTL8720DN, RTL8722DM and RTM8722CSM platform.
#endif

#ifndef RTL8720_TIMER_INTERRUPT_VERSION
  #define RTL8720_TIMER_INTERRUPT_VERSION           "RTL8720_TimerInterrupt v1.1.0"
  
  #define RTL8720_TIMER_INTERRUPT_VERSION_MAJOR     1
  #define RTL8720_TIMER_INTERRUPT_VERSION_MINOR     1
  #define RTL8720_TIMER_INTERRUPT_VERSION_PATCH     0

  #define RTL8720_TIMER_INTERRUPT_VERSION_INT       1001000  
#endif

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#include "TimerInterrupt_Generic_Debug.h"

#ifdef __cplusplus

extern "C" 
{
  #include "timer_api.h"
}
#endif

typedef enum
{
  Timer0 = GTIMER_MAX,
  Timer1 = GTIMER_MAX,
  Timer2 = 2,
  Timer3 = 3,
  MAX_TIMER = GTIMER_MAX,
} RTL8720_TimerNumber;

// TIMER0, reserved and used in us_tick(), wait_ms() functions. Users are not recommended to use
// TIMER1, used in APP_TIM_ID. Users are not recommended to use
static uint32_t timer_mapping[GTIMER_MAX] = 
{
  TIMER0,
  TIMER1,
  TIMER2,
  TIMER3
};

class RTL8720TimerInterrupt;

typedef RTL8720TimerInterrupt RTL8720Timer;

typedef void (*timerCallback)  (uint32_t data);


class RTL8720TimerInterrupt
{
  private:
    uint32_t        _timer = GTIMER_MAX;
    gtimer_t        _timerObj;

    timerCallback   _callback;        // pointer to the callback function
    float           _frequency;       // Timer frequency
    uint32_t        _timerCount;      // count to activate timer

  public:

    RTL8720TimerInterrupt(const uint8_t& timer)
    {
      if (timer >= GTIMER_MAX)
      {
        return;
      }
    
      _timer = timer;
      
      gtimer_init(&_timerObj, timer_mapping[_timer]);
      
      _callback = NULL;
    };
    
    ~RTL8720TimerInterrupt()
    {
    }

    // frequency (in hertz)
    bool setFrequency(const float& frequency, timerCallback callback)
    {
      if ( (_timer >= GTIMER_MAX) )
      {
        TISR_LOGERROR1(F("RTL8720TimerInterrupt init error, _timer >= "), GTIMER_MAX);
        
        return false;
      }
      
      // select timer frequency is 1MHz for better accuracy. We don't use 16-bit prescaler for now.
      // Will use later if very low frequency is needed.
      _frequency  = frequency;
      _timerCount = (uint32_t) (1000000.0f / _frequency);
      _callback   = callback;
      
      TISR_LOGWARN3(F("_timerNo = "), _timer, F(", _fre (Hz) = "), _frequency);
      TISR_LOGWARN3(F("Request Interval (mS) = "), _timerCount / 1000, F(", _count = "), (uint32_t) (_timerCount));

      gtimer_start_periodical(&_timerObj, _timerCount, (void *) callback, _timer);

      return true;
    }

    // interval (in microseconds)
    bool setInterval(const unsigned long& interval, timerCallback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    bool attachInterrupt(const float& frequency, timerCallback callback)
    {
      return setFrequency(frequency, callback);
    }

    // interval (in microseconds)
    bool attachInterruptInterval(const unsigned long& interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    void detachInterrupt()
    {
      gtimer_stop(&_timerObj);
    }

    void disableTimer()
    {
      gtimer_stop(&_timerObj);
    }

    void reattachInterrupt()
    {
      setFrequency(_frequency, _callback);
    }

    void enableTimer()
    {
      setFrequency(_frequency, _callback);
    }

    // Just stop clock source, clear the count
    void stopTimer()
    {
      gtimer_stop(&_timerObj);
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer()
    {
      setFrequency(_frequency, _callback);
    }
}; // class RTL8720TimerInterrupt

#endif    // RTL8720_TIMERINTERRUPT_H

