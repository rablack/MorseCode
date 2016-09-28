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

MorseCodeOutputPin::MorseCodeOutputPin(uint8_t outputPin, unsigned int _msPerTick)
  : pin(outputPin), msPerTick(_msPerTick)
{
  pinMode(outputPin, OUTPUT);
}

bool MorseCodeOutputPin::write(uint8_t value)
{
  digitalWrite(this->pin, value);
  return true;
}

// Write value to the output pin and wait for a duration of length morse-clock ticks
bool MorseCodeOutputPin::writeWithLength(uint8_t value, unsigned int length)
{
  bool result = this->write(value);
  delay(length * msPerTick);
  return result;
}

