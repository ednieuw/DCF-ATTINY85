
// Version 04. Ed Nieuwenhuys 7 may 2017
//                    ATtiny45 / ATtiny85
//
//                        |----U----|
//            Pin 5 Reset | 1 o   8 | VCC 
// (Analog input 3) Pin 3 | 2     7 | Pin 2 (Analog Input 1, SCK)
// (Analog input 2) Pin 4 | 3     6 | Pin 1 (PWM, MISO)
//                   -GND | 4     5 | Pin 0 (PWM, AREF, MOSI)
//                        |---------|
//

#include "DCF77.h"
#include "TimeLib.h"

enum PinAssignments {
  PIN01        = 0,               // 
  DCF_LEDP1    = 1,               // DCF LedPin
  PIN02        = 2,               // 
  DCF_PIN      = 3,               // DCF Pulse on interrupt pin
  PIN04        = 4,               //
  PIN05        = 5,               //
};

static unsigned long msTick;   // the number of millisecond ticks since we last incremented the second counter

//--------------------------------------------
// Heartbeat
//--------------------------------------------
uint8_t hbval = 128;
int8_t  hbdelta = 8;
static unsigned long last_time = 0;
time_t time;
byte Heartbeat = 0;

//--------------------------------------------
// DCF-2 DCF77 MODULE
//--------------------------------------------
#define DCF_INTERRUPT 0                       // Interrupt number associated with pin
byte    DCF_signal = 0;                       // is a proper time received?
DCF77   DCF = DCF77(DCF_PIN,DCF_INTERRUPT,LOW);

void setup() 
{
  DCF.Start();
  pinMode(DCF_LEDP1, OUTPUT );
  pinMode(DCF_PIN,   INPUT_PULLUP);
  analogWrite(DCF_LEDP1, 5);
  msTick = millis(); 
}

void loop() {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0)
  {
    setTime(DCFtime);
    Heartbeat = 1;
  }	
 if  (Heartbeat) heartbeat();
 else  digitalWrite(DCF_LEDP1,1-digitalRead(DCF_PIN));  //Turn on/off the led 
}

void heartbeat() 
{
  unsigned long now = millis();
  if ((now - last_time) < 40)    return;
  last_time = now;
  if (hbval > 230 || hbval < 20 ) hbdelta = -hbdelta; 
  hbval += hbdelta;
  analogWrite(DCF_LEDP1, hbval);
}



