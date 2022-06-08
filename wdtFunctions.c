/* Copyright 2019 Ron Sutton
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* These functions provide access to the features of ATSAMD51J19A Watchdog Timer peripheral */
#pragma optimize( "", off )

#include "Arduino.h"

volatile uint8_t clearWDT = 0;                 // flag set by the WDT early warning interrupt handler to inform application that it's time to clear the WDT

// wdtInit sets up and starts the WDT
void wdtInit(void) {

  MCLK->APBAMASK.reg |= MCLK_APBAMASK_WDT;                  // Activate the CLK_WDT_APB clock
  //WDT->CONFIG.bit.PER = WDT_CONFIG_PER_CYC16384;            // set WDT timeout to 16 seconds  
  WDT->CONFIG.bit.PER = WDT_CONFIG_PER_CYC8192;             // set WDT timeout to 8 seconds 
  WDT->EWCTRL.bit.EWOFFSET = WDT_EWCTRL_EWOFFSET_CYC4096;   // set WDT early warning interrupt to 4 seconds
  //WDT->CONFIG.bit.PER = WDT_CONFIG_PER_CYC4096;             // set WDT timeout to 4 seconds 
  //WDT->EWCTRL.bit.EWOFFSET = WDT_EWCTRL_EWOFFSET_CYC2048;   // set WDT early warning interrupt to 2 seconds
  //WDT->CONFIG.bit.PER = WDT_CONFIG_PER_CYC2048;             // set WDT timeout to 2 seconds 
  //WDT->EWCTRL.bit.EWOFFSET = WDT_EWCTRL_EWOFFSET_CYC1024;   // set WDT early warning interrupt to 1 seconds
  WDT->INTENSET.reg = WDT_INTENSET_EW;                      // enable the WDT early warning interrupt
  NVIC_SetPriority(WDT_IRQn, 0);                            // Set NVIC priority for WDT to 0 
  NVIC_EnableIRQ(WDT_IRQn);                                 // Connect WDT to NVIC
  
  WDT->CTRLA.reg = WDT_CTRLA_ENABLE;                        // enable the WDT
  //WDT->CTRLA.bit.ENABLE = 1;                                // enable the WDT

  while ((WDT->SYNCBUSY.reg & WDT_SYNCBUSY_ENABLE) == WDT_SYNCBUSY_ENABLE)
    ;                                                      // wait for WDT enable sync bit to go to 0

}

// This function handles the WDT Early Warning interrupt
void WDT_Handler()                                         // Called when WDT Early Warning interrupt occurs
{
  clearWDT = 1;                                            // set global flag indicating it's time to clear the WDT
  WDT->INTFLAG.bit.EW = 1;                                 // clear the EW interrupt flag
}

// This function clears the Watchdog Timer (WDT) to prevent a system restart
void wdtClear(void) {

  WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;                   // clear WDT
  while ((WDT->SYNCBUSY.reg & WDT_SYNCBUSY_CLEAR) == WDT_SYNCBUSY_CLEAR)
    ;                                                     // wait for WDT clear sync bit to go to 0

  clearWDT = 0;                                           // clear flag to indicate that WDT has been reset
}
