//
//  MorseCodeOutput.h
//  
//  Morse Code Library
//
//  Copyright Robert Black 2016.
//  All rights reserved.
// TDB: Change the license
//

#ifndef MorseCodeOutput_h
#define MorseCodeOutput_h

#include "Arduino.h"

// The MorseCodeOutput class is an abstract base class for a place to output
// morse code to.
class MorseCodeOutput
{
public:
  virtual ~MorseCodeOuput(void) { /* do nothing */ }
  virtual bool write(uint8_t value) = 0;
};

#endif /* MorseCode_h */
