/****************************************************************************
 * configs/stm32f4discovery/src/up_idle.c
 * arch/arm/src/board/up_idle.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
 *            Diego Sanchez <dsanchez@nx-engineering.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <arch/board/board.h>
#include <nuttx/config.h>

#include <nuttx/arch.h>
#include <nuttx/clock.h>

#include <debug.h>
#include <nuttx/rtc.h>
#include <arch/irq.h>

#include "up_internal.h"
#include "stm32_rcc.h"
#include "stm32_exti.h"

#include "px4fmu-v1-internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Does the board support an IDLE LED to indicate that the board is in the
 * IDLE state?
 */

#if defined(CONFIG_ARCH_LEDS) && defined(LED_IDLE)
#  define BEGIN_IDLE() up_ledon(LED_IDLE)
#  define END_IDLE()   up_ledoff(LED_IDLE)
#else
#  define BEGIN_IDLE()
#  define END_IDLE()
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#define up_idlepm()

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   up_idle() is the logic that will be executed when their is no other
 *   ready-to-run task.  This is processor idle time and will continue until
 *   some interrupt occurs to cause a context switch from the idle task.
 *
 *   Processing in this state may be processor-specific. e.g., this is where
 *   power management operations might be performed.
 *
 ****************************************************************************/

void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
  /* If the system is idle and there are no timer interrupts, then process
   * "fake" timer interrupts. Hopefully, something will wake up.
   */

  sched_process_timer();
#else

  /* Perform IDLE mode power management */

  BEGIN_IDLE();
  up_idlepm();
  END_IDLE();
#endif
}
