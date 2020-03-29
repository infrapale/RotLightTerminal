/*
 * Rotary encoder Example Program
 * HW:  using 3 digital pins of which 2 must support interrupts
 *  dpInEncoderA rotaty encoder pin, interrupt
 *  dpInEncoderB rotaty encoder pin
 *  dpInEncoderC push button pin
 *  Tom Höglund 2020
 *  https://github.com/infrapale/Rotary_Encoder.git
 */
 
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "rotenc.h"

// Digital pin definitions
enum enDigitalPins
{
  // Rotary encoder input lines
  dpInEncoderA=5,
  dpInEncoderB=6,
  dpInEncoderC=9
};

int position;

void setup()
{
    rotenc_init(dpInEncoderA, dpInEncoderB, dpInEncoderC);
    rotenc_set_step(0, 0, 10, 1);

    // init serial communication
    delay(2000);
    while (!Serial); // wait until serial console is open, remove if not tethered to computer
    Serial.begin(115200); 
    Serial.println("Ready to begin");
}


void loop()
{
  int last_pos = 0;
  while(true){
    position = get_pos();
    if(position != last_pos){
      Serial.print(last_pos);
      Serial.print(" ");
      Serial.print(position);
      Serial.print(" ");
      if(position > last_pos){
	        Serial.println("+");
      }
      else{
	        Serial.println("-");
      }
      last_pos = position;
    }
    uint8_t pressed = rd_pressed();
    if(pressed != 0){
        Serial.println(pressed);
    }
  }
}
