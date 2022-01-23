/****************************************************************************************************************************
  Change_Interval.ino
  For RTL8720DN, RTL8722DM and RTL8722CSM boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RTL8720_TimerInterrupt
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one RTL8720DN, RTL8722DM and RTL8722CSM timer and avoid conflicting 
  with other cores' tasks. The accuracy is nearly perfect compared to software timers. The most important feature is they're 
  ISR-based timers. Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
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
#define _TIMERINTERRUPT_LOGLEVEL_     3

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
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


#define TIMER0_INTERVAL_MS        500
#define TIMER1_INTERVAL_MS        1000

volatile uint32_t Timer0Count = 0;
volatile uint32_t Timer1Count = 0;

// Depending on the board, you can select RTL8720 Hardware Timer from Timer2-Timer3

// Init RTL8720 timer Timer2
RTL8720Timer ITimer0(Timer2);

// Init RTL8720 timer Timer3
RTL8720Timer ITimer1(Timer3);

void printResult(uint32_t currTime)
{
  Serial.print(F("Time = ")); Serial.print(currTime); 
  Serial.print(F(", Timer0Count = ")); Serial.print(Timer0Count);
  Serial.print(F(", Timer1Count = ")); Serial.println(Timer1Count);
}

void TimerHandler0(uint32_t timerNo)
{
  (void) timerNo;
  
  static bool toggle0 = false;

  // Flag for checking to be sure ISR is working as SErial.print is not OK here in ISR
  Timer0Count++;

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;
}

void TimerHandler1(uint32_t timerNo)
{
  (void) timerNo;
  
  static bool toggle1 = false;

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  Timer1Count++;
  
  //timer interrupt toggles outputPin
  digitalWrite(LED_BLUE, toggle1);
  toggle1 = !toggle1;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE,    OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.println(F("\nStarting Change_Interval"));
  Serial.println(RTL8720_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
 
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.print(F("Starting ITimer0 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

#define CHECK_INTERVAL_MS     10000L
#define CHANGE_INTERVAL_MS    20000L

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t lastChangeTime = 0;
  static uint32_t currTime;
  static uint32_t multFactor = 0;

  currTime = millis();

  if (currTime - lastTime > CHECK_INTERVAL_MS)
  {
    printResult(currTime);
    lastTime = currTime;

    if (currTime - lastChangeTime > CHANGE_INTERVAL_MS)
    {
      //setInterval(unsigned long interval, timerCallback callback)
      multFactor = (multFactor + 1) % 2;
      
      ITimer0.setInterval(TIMER0_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler0);
      ITimer1.setInterval(TIMER1_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler1);

      Serial.print(F("Changing Interval, Timer0 = ")); Serial.print(TIMER0_INTERVAL_MS * (multFactor + 1));
      Serial.print(F(",  Timer1 = ")); Serial.println(TIMER1_INTERVAL_MS * (multFactor + 1)); 
    
      lastChangeTime = currTime;
    }
  }
}
