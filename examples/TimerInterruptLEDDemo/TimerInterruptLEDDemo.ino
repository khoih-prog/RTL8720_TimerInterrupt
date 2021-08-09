/****************************************************************************************************************************
  TimerInterruptLEDDemo.ino
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
  1.0.0   K Hoang      30/10/2020 Initial coding for RTL8720DN, RTL8722DM and RTL8722CSM
*****************************************************************************************************************************/

/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/

#if !defined(CONFIG_PLATFORM_8721D)
  #error Only for Ameba Realtek RTL8720DN, RTL8722DM and RTM8722CSM platform.
#endif

// These define's must be placed at the beginning before #include "RTL8720_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         2
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "RTL8720_TimerInterrupt.h"
#include "RTL8720_ISR_Timer.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       PA14
#endif

#ifndef LED_BLUE
  #define LED_BLUE          PA13
#endif

#ifndef LED_RED
  #define LED_RED           PA12
#endif


#define TIMER_INTERVAL_MS         100
#define HW_TIMER_INTERVAL_MS      50

// Depending on the board, you can select RTL8720 Hardware Timer from Timer2-Timer3

// Init RTL8720 timer Timer2
RTL8720Timer ITimer(Timer2);

// Init RTL8720_ISR_Timer
// Each RTL8720_ISR_Timer can service 16 different ISR-based timers
RTL8720_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_0_5S           500L
#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_1_5S           1500L

void TimerHandler(uint32_t timerNo)
{
  (void) timerNo;
  
  ISR_Timer.run();
}

// Avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething1()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void doingSomething2()
{
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}
void doingSomething3()
{
  digitalWrite(LED_RED, !digitalRead(LED_RED));
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.println(F("\nStarting TimerInterruptLEDDemo"));
  Serial.println(RTL8720_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // configure pin in output mode
  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(LED_BLUE,     OUTPUT);
  pinMode(LED_RED,      OUTPUT);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_0_5S,  doingSomething1);
  ISR_Timer.setInterval(TIMER_INTERVAL_1S,    doingSomething2);
  ISR_Timer.setInterval(TIMER_INTERVAL_1_5S,  doingSomething3);
}


void loop()
{
  /* Nothing to do all is done by hardware. Even no interrupt required. */
}
