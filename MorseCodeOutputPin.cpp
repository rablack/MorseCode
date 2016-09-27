//
//  MorseCodeOutputPin.cpp
//  
//
//  Copyright Robert Black 2016.
//  All rights reserved
// TDB: change license
//

#include "MorseCodeOutputPin.h"
#include "Arduino.h"

MorseCodeOutputPin::MorseCodeOutputPin(uint8_t outputPin) : pin(outputPin)
{
  pinMode(outputPin, OUTPUT);
}

bool MorseCodeOutputPin::write(uint8_t value)
{
  digitalWrite(this->pin, value);
  return true;
}

