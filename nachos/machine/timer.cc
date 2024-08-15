/*! \file  timer.cc
//  \brief Routines to emulate a hardware timer device.
//
//      A hardware timer generates a CPU interrupt every X milliseconds.
//      This means it can be used for implementing time-slicing.
//
//      We emulate a hardware timer by scheduling an interrupt to occur
//      every time stats->totalTicks has increased by TIMER_TICKS.
//
//      In order to introduce some randomness into time-slicing, if "doRandom"
//      is set, then the interrupt is comes after a random number of ticks.
//
//	Remember -- nothing in here is part of Nachos.  It is just
//	an emulation for the hardware that Nachos is running on top of.

 * -----------------------------------------------------
 * This file is part of the Nachos-RiscV distribution
 * Copyright (c) 2022 University of Rennes 1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details
 * (see see <http://www.gnu.org/licenses/>).
 * -----------------------------------------------------

*/
//  DO NOT CHANGE -- part of the machine emulation
//

#include "machine/timer.h"
#include "kernel/system.h"
#include "machine/machine.h"
#include "utility/config.h"
#include "utility/stats.h"

//! dummy function because C++ does not allow pointers to member functions
static void
TimerHandler(int64_t arg) {
  Timer *p = (Timer *) arg;
  p->TimerExpired();
}

//----------------------------------------------------------------------
// Timer::Timer
/*!      Initialize a hardware timer device.  Save the place to call
//	on each interrupt, and then arrange for the timer to start
//	generating interrupts.
//
//      \param timerHandler is the interrupt handler for the timer device.
//		It is called with interrupts disabled every time the
//		the timer expires.
//      \param callArg is the parameter to be passed to the interrupt handler.
//      \param doRandom if true, arrange for the interrupts to occur
//		at random, instead of fixed, intervals.
*/
//----------------------------------------------------------------------

Timer::Timer(VoidFunctionPtr timerHandler, int callArg, bool doRandom) {
  randomize = doRandom;
  handler = timerHandler;
  arg = callArg;

  // schedule the first interrupt from the timer device
  g_machine->interrupt->Schedule(TimerHandler, (int64_t) this,
                                 TimeOfNextInterrupt(), TIMER_INT);
}

//----------------------------------------------------------------------
// Timer::TimerExpired
/*!      Routine to simulate the interrupt generated by the hardware
//	timer device.  Schedule the next interrupt, and invoke the
//	interrupt handler.
*/
//----------------------------------------------------------------------
void
Timer::TimerExpired() {
  // schedule the next timer device interrupt
  g_machine->interrupt->Schedule(TimerHandler, (int64_t) this,
                                 TimeOfNextInterrupt(), TIMER_INT);

  // invoke the Nachos interrupt handler for this device
  (*handler)(arg);
}

//----------------------------------------------------------------------
// Timer::TimeOfNextInterrupt
/*!      Return when the hardware timer device will next cause an interrupt.
//	If randomize is turned on, make it a (pseudo-)random delay.
*/
//----------------------------------------------------------------------
int
Timer::TimeOfNextInterrupt() {
  if (randomize)
    return 1 + (Random() %
                (nano_to_cycles(TIMER_TIME, g_cfg->ProcessorFrequency) * 2));
  else
    return nano_to_cycles(TIMER_TIME, g_cfg->ProcessorFrequency);
}
