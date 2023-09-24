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

// Be sure to use the correct pin numbers for your setup.
//          DA7, DA6, DA5, DA4, DA3, DA2, DA1, DA0, BDIR, BC2, BC1
AY3891x psg( 13, 12,   14,   27,   26,   25,   33,   32,   4,  16,  17);


const int clockPin = 10;
const int freq = 1000000;
const int clockChannel = 0;
const int resolution = 2;

void clockSetup()
{
  ledcSetup(clockChannel, freq, resolution);
  ledcAttachPin(clockPin, clockChannel);
  ledcWrite(clockChannel, 2);
}

const int notes_to_play[] = {
  C_4, C_4S, D_4, D_4S, E_4, F_4, F_4S, G_4, G_4S, A_4, A_4S, B_4,
  A_4S, A_4, G_4S, G_4, F_4S, F_4, E_4, D_4S, D_4, C_4S
};

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("AY-3-891x Sound Chip Library Example 3: Simple Tone Generator.");

  Serial.println("Setting up Clock");
  clockSetup();
  Serial.println("Clock Running");

  psg.begin();
  // psg.setAddress(TheChipsAddress);   // Only need this for special-ordered chips with non-default address.

  // Use less than max amplitude, in case external amp can't handle the higher level (start low and increase after testing)
  psg.write(AY3891x::ChA_Amplitude, 0x0B); // Lower amplitude
  psg.write(AY3891x::ChB_Amplitude, 0x0B); // Mid amplitude
  psg.write(AY3891x::Enable_Reg, ~(MIXER_TONE_A_DISABLE | MIXER_TONE_B_DISABLE));   // Enable Channel A and B tone generator output
  //psg.write(AY3891x::Enable_Reg, ~(MIXER_TONE_A_DISABLE));   // Enable Channel A and B tone generator output

  //psg.write(AY3891x::Enable_Reg, MIXER_ALL_DISABLED);   // Disable Tone Generators

  
}

byte i = 3;
int delayval = 40;
int iterations = 100;

void loop() {

    if ( i >= sizeof(notes_to_play) / sizeof(notes_to_play[0]) )
      i = 3;
/*
    if ( delayval-- <= 0 )
      delayval = 200;
  */    
      /*
    Serial.print("Playing note freq: ");
    Serial.print( i );
    Serial.print( " -> " );
    Serial.println(1000000UL/16/pgm_read_word(&Notes[notes_to_play[i]]));
    */
    
    psg.write(AY3891x::ChA_Tone_Period_Coarse_Reg, pgm_read_word(&Notes[notes_to_play[i]]) >> 8);
    psg.write(AY3891x::ChA_Tone_Period_Fine_Reg, pgm_read_word(&Notes[notes_to_play[i]]) & TONE_GENERATOR_FINE);
    psg.write(AY3891x::ChB_Tone_Period_Coarse_Reg, pgm_read_word(&Notes[notes_to_play[i]]) >> 8);
    psg.write(AY3891x::ChB_Tone_Period_Fine_Reg, pgm_read_word(&Notes[notes_to_play[i]]) & TONE_GENERATOR_FINE);
    delay(delayval);
    i++;
    
}
