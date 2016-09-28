//
//  MorseCodeOutputPin.h
//  
//  Morse Code Library
//
//  Copyright Robert Black 2016.
//  All rights reserved.
// TDB: Change the license
//

#ifndef MorseCodeOutputPin_h
#define MorseCodeOutputPin_h

#include "MorseCodeOutput.h"
#include "Arduino.h"

// The MorseCodeOutputPin class is an implementation of MorseCodeOutput
// that outputs morse code on a pin.
class MorseCodeOutputPin : public MorseCodeOutput
{
public:
  MorseCodeOutputPin(uint8_t pinNumber, unsigned int _msPerTick = 250);
  bool write(uint8_t value);
  bool writeWithLength(uint8_t value, unsigned int length);

private:
  uint8_t pin;
  unsigned int msPerTick;
};

#endif /* MorseCode_h */
