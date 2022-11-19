# RTL8720_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/RTL8720_TimerInterrupt.svg?)](https://www.ardu-badge.com/RTL8720_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/RTL8720_TimerInterrupt.svg)](https://github.com/khoih-prog/RTL8720_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/RTL8720_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/RTL8720_TimerInterrupt.svg)](http://github.com/khoih-prog/RTL8720_TimerInterrupt/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-RTL8720_TimerInterrupt/count.svg" title="RTL8720_TimerInterrupt Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-RTL8720_TimerInterrupt/count.svg" style="height: 30px;width: 200px;"></a>


---
---

## Table of Contents

* [Important Change from v1.1.0](#Important-Change-from-v110)
* [Why do we need this RTL8720_TimerInterrupt library](#why-do-we-need-this-rtl8720_timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information about RTL8720 Timers](#more-useful-information-about-rtl8720-timers)
* [New from v1.0.0](#new-from-v100)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Init Hardware Timer and ISR-based Timer](#21-init-hardware-timer-and-isr-based-timer)
    * [2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#22-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_None](examples/Argument_None)
  * [  2. ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
  * [  3. TimerInterruptTest](examples/TimerInterruptTest)
  * [  4. TimerInterruptLEDDemo](examples/TimerInterruptLEDDemo)
  * [  5. **Change_Interval**](examples/Change_Interval).
  * [  6. **ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex)
  * [  7. **multiFileProject**](examples/multiFileProject) **New**
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on RTL8720DN](#1-isr_16_timers_array_complex-on-rtl8720dn)
  * [2. TimerInterruptTest on RTL8720DN](#2-timerinterrupttest-on-rtl8720dn)
  * [3. Argument_None on RTL8720DN](#3-argument_none-on-rtl8720dn)
  * [4. Change_Interval on RTL8720DN](#4-change_interval-on-rtl8720dn)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v1.1.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)


### Why do we need this [RTL8720_TimerInterrupt library](https://github.com/khoih-prog/RTL8720_TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on an RTL8720-based board, such as RTL8720DN, RTL8722DM and RTL8722CSM. 

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long milliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

#### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **RTL8720DN, RTL8722DM, RTM8722CSM, etc. boards**

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino AmebaD core 3.1.2+`](https://github.com/ambiot/ambd_arduino) for Realtek RTL8720DN, RTL8722DM and RTM8722CSM. [![GitHub release](https://img.shields.io/github/release/ambiot/ambd_arduino.svg)](https://github.com/ambiot/ambd_arduino/releases/latest)
 3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples.
   
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**RTL8720_TimerInterrupt**](https://github.com/khoih-prog/RTL8720_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/RTL8720_TimerInterrupt.svg?)](https://www.ardu-badge.com/RTL8720_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**RTL8720_TimerInterrupt**](https://github.com/khoih-prog/RTL8720_TimerInterrupt) page.
2. Download the latest release `RTL8720_TimerInterrupt-master.zip`.
3. Extract the zip file to `RTL8720_TimerInterrupt-master` directory 
4. Copy whole `RTL8720_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**RTL8720_TimerInterrupt** library](https://platformio.org/lib/show/12662/RTL8720_TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/12662/RTL8720_TimerInterrupt/installation). Search for **RTL8720_TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include these `.hpp` or `.h` files

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RTL8720_TimerInterrupt.h"     //https://github.com/khoih-prog/RTL8720_TimerInterrupt

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RTL8720_ISR_Timer.hpp"         //https://github.com/khoih-prog/RTL8720_TimerInterrupt
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "RTL8720_ISR_Timer.h"           //https://github.com/khoih-prog/RTL8720_TimerInterrupt
```


---
---


## More useful Information about RTL8720 Timers

There are only 4 hardware timers in RTL8720DN, RTL8722DM and RTL8722CSM.

**TIMER0**, reserved and used in us_tick(), wait_ms() functions. Users are not recommended to use.

**TIMER1**, used in APP_TIM_ID. Users are not recommended to use

**TIMER2 and TIMER3** are ready and OK to be used.


<p align="center">
    <img src="https://github.com/khoih-prog/RTL8720_TimerInterrupt/blob/main/pics/Rtlduino_RTL8720DN_Pinout.png">
</p>


---
---

## New from v1.0.0

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long milliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 

---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Init RTL8720 timer Timer2
RTL8720Timer ITimer(Timer2);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler0(void)
{
  // Doing something here inside ISR
}

#define TIMER0_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timer


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// Depending on the board, you can select RTL8720 Hardware Timer from Timer2-Timer3

// Init RTL8720 timer Timer2
RTL8720Timer ITimer(Timer2);

// Init RTL8720_ISR_Timer
// Each RTL8720_ISR_Timer can service 16 different ISR-based timers
RTL8720_ISR_Timer ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_US          100L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// Avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR
}
  
void doingSomething5s()
{
  // Doing something here inside ISR
}

void doingSomething11s()
{
  // Doing something here inside ISR
}

void doingSomething101s()
{
  // Doing something here inside ISR
}

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```


---
---

### Examples: 

 1. [Argument_None](examples/Argument_None)
 2. [ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
 3. [TimerInterruptTest](examples/TimerInterruptTest)
 4. [TimerInterruptLEDDemo](examples/TimerInterruptLEDDemo)
 5. [**Change_Interval**](examples/Change_Interval). New
 6. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
 7. [**multiFileProject**](examples/multiFileProject). **New**

---
---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```
#if !defined(CONFIG_PLATFORM_8721D)
  #error Only for Ameba Realtek RTL8720DN, RTL8722DM and RTM8722CSM platform.
#endif

// These define's must be placed at the beginning before #include "RTL8720_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         2
#define _TIMERINTERRUPT_LOGLEVEL_     3

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RTL8720_TimerInterrupt.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "RTL8720_ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       PA14
#endif

#ifndef LED_BLUE
  #define LED_BLUE          PA13
#endif

#ifndef LED_RED
  #define LED_RED           PA12
#endif

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

// Depending on the board, you can select RTL8720 Hardware Timer from Timer2-Timer3

// Init RTL8720 timer Timer2
RTL8720Timer ITimer(Timer2);

// Init RTL8720_ISR_Timer
// Each RTL8720_ISR_Timer can service 16 different ISR-based timers
RTL8720_ISR_Timer ISR_Timer;


#define LED_TOGGLE_INTERVAL_MS        2000L

void TimerHandler(uint32_t timerNo)
{
  (void) timerNo;
  
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  ();

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

  typedef struct 
  {
    irqCallback   irqCallbackFunc;
    uint32_t      TimerInterval;
    unsigned long deltaMillis;
    unsigned long previousMillis;
  } ISRTimerData;
  
  // Avoid doing something fancy in ISR, for example Serial.print()
  // The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
  // Or you can get this run-time error / crash
  
  void doingSomething(int index);

#else

  volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  // You can assign any interval for any timer here, in milliseconds
  uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
  {
    5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
    45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    deltaMillis[index]    = currentMillis - previousMillis[index];
    previousMillis[index] = currentMillis;
  }

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

  ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
  {
    //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
    { doingSomething0,    5000L, 0, 0 },
    { doingSomething1,   10000L, 0, 0 },
    { doingSomething2,   15000L, 0, 0 },
    { doingSomething3,   20000L, 0, 0 },
    { doingSomething4,   25000L, 0, 0 },
    { doingSomething5,   30000L, 0, 0 },
    { doingSomething6,   35000L, 0, 0 },
    { doingSomething7,   40000L, 0, 0 },
    { doingSomething8,   45000L, 0, 0 },
    { doingSomething9,   50000L, 0, 0 },
    { doingSomething10,  55000L, 0, 0 },
    { doingSomething11,  60000L, 0, 0 },
    { doingSomething12,  65000L, 0, 0 },
    { doingSomething13,  70000L, 0, 0 },
    { doingSomething14,  75000L, 0, 0 },
    { doingSomething15,  80000L, 0, 0 }
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
    curISRTimerData[index].previousMillis = currentMillis;
  }

#else

  irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
  {
    doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
    doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
    doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
    doingSomething12, doingSomething13, doingSomething14, doingSomething15
  };

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  Serial.print(F("SimpleTimer : "));Serial.print(SIMPLE_TIMER_MS / 1000);
  Serial.print(F(", ms : ")); Serial.print(currMillis);
  Serial.print(F(", Dms : ")); Serial.println(currMillis - previousMillis);

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(curISRTimerData[i].TimerInterval);
    Serial.print(F(", actual : ")); Serial.println(curISRTimerData[i].deltaMillis);
#else
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(TimerInterval[i]);
    Serial.print(F(", actual : ")); Serial.println(deltaMillis[i]);
#endif    
  }

  previousMillis = currMillis;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.println(F("\nStarting ISR_16_Timers_Array_Complex"));
  Serial.println(RTL8720_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(startMillis);
  }
  else
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each STM32_ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif    
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

### 1. ISR_16_Timers_Array_Complex on RTL8720DN

The following is the sample terminal output when running new example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **Rtlduino RTL8720DN (BW16)** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy or blocked**. The 16 independent ISR timers are **programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!**

While software timer, **programmed for 2s, is activated after 10.003s in loop()!!!**.

In this example, 16 independent ISR Timers are used, yet utilized just one Hardware Timer. The Timer Intervals and Function Pointers are stored in arrays to facilitate the code modification.

```
Starting ISR_16_Timers_Array_Complex
RTL8720_TimerInterrupt v1.1.0
CPU Frequency = 200 MHz
[TISR] _timerNo = 2, _fre (Hz) = 100.00
[TISR] Request Interval (mS) = 10, _count = 10000
Starting ITimer OK, millis() = 117
SimpleTimer : 2, ms : 10120, Dms : 10003
Timer : 0, programmed : 5000, actual : 4992
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20184, Dms : 10064
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10003
Timer : 2, programmed : 15000, actual : 15005
Timer : 3, programmed : 20000, actual : 20007
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30249, Dms : 10065
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20007
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30011
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40314, Dms : 10065
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9994
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30011
Timer : 6, programmed : 35000, actual : 35003
Timer : 7, programmed : 40000, actual : 40005
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50380, Dms : 10066
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 10003
Timer : 2, programmed : 15000, actual : 14996
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 24999
Timer : 5, programmed : 30000, actual : 30011
Timer : 6, programmed : 35000, actual : 35003
Timer : 7, programmed : 40000, actual : 40005
Timer : 8, programmed : 45000, actual : 45007
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60447, Dms : 10067
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15005
Timer : 3, programmed : 20000, actual : 20007
Timer : 4, programmed : 25000, actual : 24999
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35003
Timer : 7, programmed : 40000, actual : 40005
Timer : 8, programmed : 45000, actual : 45007
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55010
Timer : 11, programmed : 60000, actual : 60012
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70514, Dms : 10067
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9994
Timer : 2, programmed : 15000, actual : 15005
Timer : 3, programmed : 20000, actual : 20007
Timer : 4, programmed : 25000, actual : 24999
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35003
Timer : 7, programmed : 40000, actual : 40005
Timer : 8, programmed : 45000, actual : 45007
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55010
Timer : 11, programmed : 60000, actual : 60012
Timer : 12, programmed : 65000, actual : 65004
Timer : 13, programmed : 70000, actual : 70006
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80582, Dms : 10068
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 14996
Timer : 3, programmed : 20000, actual : 19998
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35003
Timer : 7, programmed : 40000, actual : 40005
Timer : 8, programmed : 45000, actual : 45007
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55010
Timer : 11, programmed : 60000, actual : 60012
Timer : 12, programmed : 65000, actual : 65004
Timer : 13, programmed : 70000, actual : 70006
Timer : 14, programmed : 75000, actual : 75008
Timer : 15, programmed : 80000, actual : 80010
```

---

### 2. TimerInterruptTest on RTL8720DN

The following is the sample terminal output when running example [**TimerInterruptTest**](examples/TimerInterruptTest) on **Rtlduino RTL8720DN (BW16)** to demonstrate how to start/stop Hardware Timers.

```

Starting TimerInterruptTest
RTL8720_TimerInterrupt v1.1.0
CPU Frequency = 200 MHz
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
Starting ITimer0 OK, millis() = 119
[TISR] _timerNo = 3, _fre (Hz) = 0.33
[TISR] Request Interval (mS) = 3000, _count = 3000000
Starting  ITimer1 OK, millis() = 131
ITimer0 called, millis() = 1055
ITimer0 called, millis() = 2053
ITimer0 called, millis() = 2989
ITimer1 called, millis() = 2999
ITimer0 called, millis() = 3924
ITimer0 called, millis() = 4860
Stop ITimer0, millis() = 5003
ITimer1 called, millis() = 5870
ITimer1 called, millis() = 8681
Start ITimer0, millis() = 10004
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
ITimer0 called, millis() = 10951
ITimer1 called, millis() = 11493
ITimer0 called, millis() = 11886
ITimer0 called, millis() = 12822
ITimer0 called, millis() = 13757
ITimer1 called, millis() = 14299
ITimer0 called, millis() = 14692
Stop ITimer1, millis() = 15003
Stop ITimer0, millis() = 15005
Start ITimer0, millis() = 20006
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
ITimer0 called, millis() = 21015
ITimer0 called, millis() = 21953
ITimer0 called, millis() = 22889
ITimer0 called, millis() = 23825
ITimer0 called, millis() = 24761
Stop ITimer0, millis() = 25007
Start ITimer1, millis() = 30004
[TISR] _timerNo = 3, _fre (Hz) = 0.33
[TISR] Request Interval (mS) = 3000, _count = 3000000
Start ITimer0, millis() = 30015
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
ITimer0 called, millis() = 30962
ITimer0 called, millis() = 31898
ITimer1 called, millis() = 32824
ITimer0 called, millis() = 32833
ITimer0 called, millis() = 33769
ITimer0 called, millis() = 34705
Stop ITimer0, millis() = 35016
ITimer1 called, millis() = 35692
ITimer1 called, millis() = 38504
Start ITimer0, millis() = 40017
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
ITimer0 called, millis() = 40964
ITimer1 called, millis() = 41315
ITimer0 called, millis() = 41898
ITimer0 called, millis() = 42834
```

---

### 3. Argument_None on RTL8720DN

The following is the sample terminal output when running example [**Argument_None**](examples/Argument_None) on **Rtlduino RTL8720DN (BW16)** to demonstrate how to start/stop Multiple Hardware Timers.

```
Starting Argument_None
RTL8720_TimerInterrupt v1.1.0
CPU Frequency = 200 MHz
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
Starting ITimer0 OK, millis() = 116
[TISR] _timerNo = 3, _fre (Hz) = 0.20
[TISR] Request Interval (mS) = 5000, _count = 5000000
Starting ITimer1 OK, millis() = 127
Using timer = 2, ITimer0: millis() = 1116, delta = 1000
Using timer = 2, ITimer0: millis() = 2112, delta = 996
Using timer = 2, ITimer0: millis() = 3108, delta = 996
Using timer = 2, ITimer0: millis() = 4104, delta = 996
Using timer = 2, ITimer0: millis() = 5100, delta = 996
Using timer = 3, ITimer1: millis() = 5107, delta = 4980
Using timer = 2, ITimer0: millis() = 6092, delta = 992
Using timer = 2, ITimer0: millis() = 7088, delta = 996
Using timer = 2, ITimer0: millis() = 8084, delta = 996
Using timer = 2, ITimer0: millis() = 9080, delta = 996
Using timer = 2, ITimer0: millis() = 10076, delta = 996
Using timer = 3, ITimer1: millis() = 10083, delta = 4976
Using timer = 2, ITimer0: millis() = 11068, delta = 992
Using timer = 2, ITimer0: millis() = 12064, delta = 996
Using timer = 2, ITimer0: millis() = 13060, delta = 996
Using timer = 2, ITimer0: millis() = 14056, delta = 996
Using timer = 2, ITimer0: millis() = 15052, delta = 996
Using timer = 3, ITimer1: millis() = 15059, delta = 4976
Using timer = 2, ITimer0: millis() = 16044, delta = 992
Using timer = 2, ITimer0: millis() = 17040, delta = 996
Using timer = 2, ITimer0: millis() = 18036, delta = 996
Using timer = 2, ITimer0: millis() = 19032, delta = 996
Using timer = 2, ITimer0: millis() = 20028, delta = 996
Using timer = 3, ITimer1: millis() = 20035, delta = 4976
```

---

### 4. Change_Interval on RTL8720DN

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on **Rtlduino RTL8720DN (BW16)** to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval
RTL8720_TimerInterrupt v1.1.0
CPU Frequency = 200 MHz
[TISR] _timerNo = 2, _fre (Hz) = 2.00
[TISR] Request Interval (mS) = 500, _count = 500000
Starting ITimer0 OK, millis() = 119
[TISR] _timerNo = 3, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
Starting ITimer1 OK, millis() = 130
Time = 10001, Timer0Count = 19, Timer1Count = 9
Time = 20002, Timer0Count = 39, Timer1Count = 19
[TISR] _timerNo = 2, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
[TISR] _timerNo = 3, _fre (Hz) = 0.50
[TISR] Request Interval (mS) = 2000, _count = 2000000
Changing Interval, Timer0 = 1000,  Timer1 = 2000
Time = 30003, Timer0Count = 48, Timer1Count = 23
Time = 40004, Timer0Count = 58, Timer1Count = 28
[TISR] _timerNo = 2, _fre (Hz) = 2.00
[TISR] Request Interval (mS) = 500, _count = 500000
[TISR] _timerNo = 3, _fre (Hz) = 1.00
[TISR] Request Interval (mS) = 1000, _count = 1000000
Changing Interval, Timer0 = 500,  Timer1 = 1000
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level (_TIMERINTERRUPT_LOGLEVEL_) from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "RTL8720_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.

---
---

### Issues

Submit issues to: [RTL8720_TimerInterrupt issues](https://github.com/khoih-prog/RTL8720_TimerInterrupt/issues)

---

## TO DO

1. Search for bug and improvement.

---

## DONE

1. Basic hardware timers for RTL8720.
2. More hardware-initiated software-enabled timers
3. Longer time interval
4. Similar features for remaining Arduino boards such as ESP32, ESP8266, SAMD21, SAMD51, nRF52, mbed-nRF52, Teensy, etc.
5. Add Table of Contents
6. Fix `multiple-definitions` linker error
7. Optimize library code by using `reference-passing` instead of `value-passing`


---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/RTL8720_TimerInterrupt/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


