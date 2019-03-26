/* Copyright 2019 Ron Sutton
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

extern "C"{
  #include "wdtFunctions.h"
}



void setup() {

  Serial.begin(500000);                        // Initialise the Serial port
  while(!Serial);                              // Wait for the console to be started

  Serial.println("Initializing WDT");
  
  wdtInit();                                   // enable WDT
  
}

void loop() {

  uint32_t cnt = 0;

  while (1) {

    Serial.print("Waiting for WDT EW interrupt #");
    Serial.println(++cnt);
    
    // wait for early warning interrupt
    while (clearWDT == 0) ;

    Serial.println("WDT early warning interrupt detected");

    if (Serial.available()) {                  // if any character is entered...
      char c = Serial.read();
      Serial.print("Waiting for system reset");
      while (1) {                              //  don't clear WDT so we can see if system is reset.
        Serial.print(".");
        delay(500);
      }
    } else {
      wdtClear();
      Serial.println("WDT cleared");
    } 
  }
}


