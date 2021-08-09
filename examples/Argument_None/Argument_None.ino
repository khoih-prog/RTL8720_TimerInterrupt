/****************************************************************************************************************************
  Argument_None.ino
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
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "RTL8720_TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       PA14
#endif

#ifndef LED_BLUE
  #define LED_BLUE          PA13
#endif

#ifndef LED_RED
  #define LED_RED           PA12
#endif

uint32_t startTimer0Millis;
uint32_t startTimer1Millis;

void  TimerHandler0(uint32_t timerNo)
{
  static bool toggle0 = false;

#if (TIMER_INTERRUPT_DEBUG > 0)
  static uint32_t preMillis = 0;
  static uint32_t curMillis = 0;

  if (preMillis == 0)
    preMillis = startTimer0Millis;
  
  curMillis = millis();

  Serial.print(F("Using timer = ")); Serial.print(timerNo);
  
  Serial.print(F(", ITimer0: millis() = ")); Serial.print(curMillis);
  Serial.print(F(", delta = ")); Serial.println(curMillis - preMillis);
  
  preMillis = curMillis;
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;
}

void  TimerHandler1(uint32_t timerNo)
{
  static bool toggle1 = false;

#if (TIMER_INTERRUPT_DEBUG > 0)
  static uint32_t preMillis = 0;
  static uint32_t curMillis = 0;

  if (preMillis == 0)
    preMillis = startTimer1Millis;
  
  curMillis = millis();

  Serial.print(F("Using timer = ")); Serial.print(timerNo);
  
  Serial.print(F(", ITimer1: millis() = ")); Serial.print(curMillis);
  Serial.print(F(", delta = ")); Serial.println(curMillis - preMillis);
  
  preMillis = curMillis;
#endif

  //timer interrupt toggles outputPin
  digitalWrite(LED_BLUE, toggle1);
  toggle1 = !toggle1;
}

#define TIMER0_INTERVAL_MS        1000
#define TIMER1_INTERVAL_MS        5000

// Depending on the board, you can select RTL8720 Hardware Timer from Timer2-Timer3

// Init RTL8720 timer Timer2
RTL8720Timer ITimer0(Timer2);

// Init RTL8720 timer Timer3
RTL8720Timer ITimer1(Timer3);

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  delay(100);

  Serial.println(F("\nStarting Argument_None"));
  Serial.println(RTL8720_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    startTimer0Millis = millis();
    Serial.print(F("Starting ITimer0 OK, millis() = ")); Serial.println(startTimer0Millis);
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    startTimer1Millis = millis();
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(startTimer1Millis);
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{

}
