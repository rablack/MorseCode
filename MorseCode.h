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
  static String encode(String text);
};

#endif /* MorseCode_h */
