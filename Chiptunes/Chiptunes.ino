/* -----------------------------------------------------------------
   AY3891x Library Example Sketch 3: Simple Tone Generator
   https://github.com/Andy4495/AY3891x

   This code will play through the middle-octave notes C4 to C5.
   It assumes a 1 MHz clock input to the AY-3-891x chip.

   There is hardware-specific code to have the microcontroller
   generate the 1 MHz clock. If you are not using an Atmega328 or 32U4
   device, then you will need to provide your own code or clock signal
   to generate the required clock, and comment out
   // #define HARDWARE_GENERATED_CLOCK

   12/21/20 - A.T. - Original
   08/24/21 - A.T. - Support PROGMEM for Notes[] array

*/

#include "AY3891x.h"
#include "AY3891x_sounds.h"  // Contains the divisor values for the musical notes
//#include "chiptunes_f15.h"
//#include "chiptunes_ultima.h"
//#include "chiptunes_wolfgang.h"
//#include "chiptunes_testdrive.h"
//#include "chiptunes_lemmings.h"
#include "chiptunes_drwho.h"
//#include "chiptunes_hype.h"
//#include "chiptunes_aura.h"


size_t data_index = 0;
unsigned long prev_micros;
// Write the data at a rate of 50 Hz <-> every 20000 us
#define INTERVAL 20000

#ifdef ESP32

AY3891x psg( 13, 12,   14,   27,   26,   25,   33,   32,   4,  16,  17);
const int clockPin = 10;
const int freq = 3000000;
const int clockChannel = 0;
const int resolution = 2;

void clockSetup()
{
  ledcSetup(clockChannel, freq, resolution);
  ledcAttachPin(clockPin, clockChannel);
  ledcWrite(clockChannel, 2);
}

#else

AY3891x psg( A3,   8,   7,   6,   5,   4,   3,   2,   A2,  A1,  A0);

// The following code generates a 1 MHz 50% duty cycle output to be used
// as the clock signal for the AY-3-891x chip.
// Note that the following code is hardware-specific. It works on certain Atmega
// chips (including Arduino UNO), but will not work on all microcontrollers
// without modification
static const byte clkOUT = 9;
const byte DIVISOR = 3; // Set for 1MHz clock
static void clockSetup()
{
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12) | (1 << CS10);
  TCCR1C = 0;
  TIMSK1 = 0;
  OCR1AH = 0;
  OCR1AL = DIVISOR;
}

#endif

void setup() {
#ifdef ESP32
  clockSetup();
#else
  // Hardware-specific microcontroller code to generate a clock signal for the AY-3-891x chip
  pinMode(clkOUT, OUTPUT);
  digitalWrite(clkOUT, LOW);
  clockSetup();
#endif


  Serial.begin(38400);
  Serial.println("");
  Serial.println("AY-3-891x Sound Chip Library Example 3: Simple Tone Generator.");

  psg.begin();

}


byte i = 3;
int delayval = 50;
int iterations = 100;

void loop()
{
  byte i;

  if (micros() - prev_micros > INTERVAL)
  {
    prev_micros = micros();

    for (i= 0; i < 14; i++) {
      psg.write(i, pgm_read_byte(&psg_data[data_index++]));
    }

    if (data_index >= sizeof(psg_data)) {
      psg.write(AY3891x::Enable_Reg, MIXER_NOISES_DISABLE | MIXER_TONES_DISABLE | psg.read(AY3891x::Enable_Reg));
      while (1);
      // Or... you can re-start the song:
      //  data_index = 0;
    }
  }

}
