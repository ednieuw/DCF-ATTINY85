
//                    ATtiny45 / ATtiny85
//
//                        |----U----|
//            Pin 5 Reset | 1 o   8 | VCC 
// (Analog input 3) Pin 3 | 2     7 | Pin 2 (Analog Input 1, SCK)
// (Analog input 2) Pin 4 | 3     6 | Pin 1 (PWM, MISO)
//                   -GND | 4     5 | Pin 0 (PWM, AREF, MOSI)
//                        |---------|
//
//

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "DCF77.h"
//#include "TimeLib.h"
#include <Manchester.h>

enum PinAssignments {
  TX_PIN       = 0,               // 433MHz Transmit
  DCF_LEDP1    = 1,               // DCF LedPin
  PIN02        = 2,               // Not Used, Analog Input 
  DCF_PIN      = 3,               // DCF input, Pulse on interrupt pin
  PIN04        = 4,               // Not Used, Analog input
  ResetPin     = 5,               // Not Used, Reset
};

#define TX_PIN  0  //pin where your transmitter is connected
#define LED_PIN 1 //pin for blinking LED

uint8_t moo = 1; //last led status
uint8_t data[20] = {11, '1','2', '3', '4', '5', '6', '7', '8', '9','1','2','3','4','5','6','7','8','9'};

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_9600);
}


uint8_t datalength=2;   //at least two data
void loop() 
{

  data[0] = datalength;

  man.transmitArray(datalength, data);
  moo = ++moo % 2;
  digitalWrite(LED_PIN, moo);


  delay(800);
  datalength++;
  if(datalength>18) datalength=2;
}
