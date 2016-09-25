//
//  MorseCode.h
//  
//  Morse Code Library
//
//  Copyright Robert Black 2016.
//  All rights reserved.
// TDB: Change the license
//

#ifndef MorseCode_h
#define MorseCode_h

class String;

// The MorseCode class will handle asynchronous transmission of morse code.
// Today only encoding of messages into morse dots and dashes is implemented.
class MorseCode
{
public:
  // Options for dealing with invalid characters
  enum InvalidChar
  {
    IgnoreChar, // Don't output anything
    TransmitErrorCode // Output the morse error sequence (8 dots)
  };

  MorseCode(InvalidChar behavior = TransmitErrorCode);

  bool encode(String& result, const String text);
  void setInvalidCharBehavior(InvalidChar newBehavior);
  InvalidChar getInvalidCharBehavior(void) const;

private:
  InvalidChar invalidCharBehavior;
};

#endif /* MorseCode_h */
