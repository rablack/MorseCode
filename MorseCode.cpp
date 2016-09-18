//
//  MorseCode.cpp
//  
//
//  Copyright Robert Black 2016.
//  All rights reserved
// TDB: change license
//

#include "MorseCode.h"
#include "Arduino.h"

// Entry in a switch statement to encode a character into a morse sequence
#define MORSE_ENTRY(CHAR,MORSESEQ) case (CHAR): encoded += F(MORSESEQ); break;

// Encode a string into a morse sequence
String MorseCode::encode(String message)
{
  String encoded = "";

  // Add a space between sequences for each letter
  for (int i = 0; i < message.length(); i++) {
    if (i != 0) {
      encoded += " ";
    }

    // Get the next character and convert to lower case
    char c = message.charAt(i);
    if (isUpperCase(c)) {
      c = c - 'A' + 'a';
    }

    // Look up the morse sequence for the character
    switch(c) {
      // Letters
      MORSE_ENTRY('a', ".-");
      MORSE_ENTRY('b', "-...");
      MORSE_ENTRY('c', "-.-.");
      MORSE_ENTRY('d', "-..");
      MORSE_ENTRY('e', ".");
      MORSE_ENTRY('f', "..-.");
      MORSE_ENTRY('g', "--.");
      MORSE_ENTRY('h', "....");
      MORSE_ENTRY('i', "..");
      MORSE_ENTRY('j', ".---");
      MORSE_ENTRY('k', "-.-");
      MORSE_ENTRY('l', ".-..");
      MORSE_ENTRY('m', "--");
      MORSE_ENTRY('n', "-.");
      MORSE_ENTRY('o', "---");
      MORSE_ENTRY('p', ".--.");
      MORSE_ENTRY('q', "--.-");
      MORSE_ENTRY('r', ".-.");
      MORSE_ENTRY('s', "...");
      MORSE_ENTRY('t', "-");
      MORSE_ENTRY('u', "..-");
      MORSE_ENTRY('v', "...-");
      MORSE_ENTRY('w', ".--");
      MORSE_ENTRY('x', "-..-");
      MORSE_ENTRY('y', "-.--");
      MORSE_ENTRY('z', "--..");
      
      // Numbers
      MORSE_ENTRY('0', "-----");
      MORSE_ENTRY('1', ".----");
      MORSE_ENTRY('2', "..---");
      MORSE_ENTRY('3', "...--");
      MORSE_ENTRY('4', "....-");
      MORSE_ENTRY('5', ".....");
      MORSE_ENTRY('6', "-....");
      MORSE_ENTRY('7', "--...");
      MORSE_ENTRY('8', "---..");
      MORSE_ENTRY('9', "----.");
      
      // Punctuation
      MORSE_ENTRY('.', ".-.-.-");
      MORSE_ENTRY(',', "--..--");
      MORSE_ENTRY(':', "---...");
      MORSE_ENTRY('?', "..--..");
      MORSE_ENTRY('\'', ".----.");
      MORSE_ENTRY('-', "-....-");
      MORSE_ENTRY('/', "-..-.");
      MORSE_ENTRY('(', "-.--.");
      MORSE_ENTRY(')', "-.--.-");
      MORSE_ENTRY('"', ".-..-.");
      MORSE_ENTRY('=', "-...-");
      MORSE_ENTRY('+', ".-.-.");
      MORSE_ENTRY('*', "-..-");
      MORSE_ENTRY('@', ".--.-.");

      // Space encodes as a space for now. This will have an inter-letter
      // space on either side so it will become 3 spaces in the output
      // unless it is at the start or the end of a message. A space at the
      // start or end is indistinguishable from the surrounding lack of output.
      // At some point a test will be needed to make sure that consecutive
      // messages have at least a space's gap between them (the length of
      // 7 dots).
      MORSE_ENTRY(' ', " ");
    }
  }
  return encoded;
}
