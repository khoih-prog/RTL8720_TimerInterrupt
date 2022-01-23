/****************************************************************************************************************************
  RTL8720_ISR_Timer.h
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

#ifndef ISR_TIMER_GENERIC_H
#define ISR_TIMER_GENERIC_H

#include "RTL8720_ISR_Timer.hpp"
#include "RTL8720_ISR_Timer-Impl.h"

#endif    // ISR_TIMER_GENERIC_H

