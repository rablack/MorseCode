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

const int MORSE_DASH_TICKS = 3;
const int MORSE_DOT_TICKS = 1;

//==== Constructors and destructors ====
MorseCode::MorseCode(MorseCodeOutput* morseOutput, InvalidChar behavior) :
  output(morseOutput),
  invalidCharBehavior(behavior)
{
  // Nothing else to do
}

//==== Methods====

// Write a message in morse code on the output.
//
// return true for success, false for failure. Note that this is inconsistent with
// the normal convention of write() returning the number of characters written. When
// this is refactored to cope with non-blocking writes the interface will have to change.
bool MorseCode::write(const String& message)
{
  String encoded;
  // encode() is independently testable.
  bool result = this->encode(encoded, message);
  
  // Note the use of bitwise &= as opposed to logical && to avoid lazy evaluation.
  // The currently defined behavior is that sendCode() should be called
  // regardless of the value of result.
  result &= this->sendCode(encoded);
  
  return result;
}

// Send a sequence of dots and dashes to the output object.
// This method adds an inter-letter space to the end of every non-empty sequence.
bool MorseCode::sendCode(const String& code)
{
  bool result = true;
  String spacedCode = code;
  // Add inter-letter space, if needed.
  if (code.length() > 0) {
    spacedCode += " ";
  }
  
  // The following section defines "correct behaviour" in terms of timing. This is
  // unnecessarily hard to test.
  // It also contains a bug that it does not take any notice of the return value
  // of output->write()
  if (this->output != NULL) {
    for (int i = 0; i < spacedCode.length(); i++) {
      bool success = false; // Was outputting this character successful?
      switch(spacedCode.charAt(i)) {
      case '-':
        success = output->writeWithLength(HIGH, MORSE_DASH_TICKS);
        break;

      case '.':
        success = output->writeWithLength(HIGH, MORSE_DOT_TICKS);
        break;

      case ' ':
        success = output->writeWithLength(LOW, MORSE_DOT_TICKS);
        break;
      }
      // Output inter-signal gap.
      if (!output->writeWithLength(LOW, MORSE_DOT_TICKS)) {
        result = false;
      }
      result = result && success;
    }
  } else {
    // No output to write to. Return false to indicate an error.
    result = false;
  }
  return result;
}

// Entry in a switch statement to encode a character into a morse sequence
#define MORSE_ENTRY(CHAR,MORSESEQ) case (CHAR): encoded += F(MORSESEQ); break;

// Encode a string into a morse sequence
bool MorseCode::encode(String& encoded, const String message)
{
  encoded = "";
  bool result = true;

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

    default:
      // The character is unrecognised so set the result to false to
      // indicate failure.
      result = false;

      // If appropriate transmit the morse error sequence (8 dots).
      // Otherwise remove the trailing inter-letter space (if it exists).
      if (this->getInvalidCharBehavior() == TransmitErrorCode) {
        encoded += F("........");
      } else {
        int length = encoded.length();
        if (length > 0) {
          encoded.remove(length - 1);
        } 
      }
    }
  }
  return result;
}

// Setter method for invalidCharBehavior
//
// Changes the behavior when an invalid character is encountered.
void MorseCode::setInvalidCharBehavior(InvalidChar behavior)
{
  invalidCharBehavior = behavior;
}

// Getter method for invalidCharBehavior
//
// returns the currently configured behavior for what to do when encountering
// an invalid character.
MorseCode::InvalidChar MorseCode::getInvalidCharBehavior(void) const
{
  return invalidCharBehavior;
}
