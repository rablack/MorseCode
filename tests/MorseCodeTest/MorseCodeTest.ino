#line 2 "MorseCodeTest.ino"
#include "ArduinoUnit.h"

// MorseCodeTest
//
// Unit tests for the MorseCode class
//
// Copyright Robert Black 2016
// All rights reserved.
// TDB: change license to something open-source

#include "MorseCode.h"

// Mock object to test the interface between MorseCode and MorseCodeOutput
// This is deliberately a struct because the test cases need access to the internals
struct MockMorseCodeOutput : public MorseCodeOutput
{
  MockMorseCodeOutput(void) : flashes(0), successFlag(true), lastValue(LOW) {}

  // Count the number of calls with a HIGH value: the total number of dots
  // and dashes in the message.
  // Store the value passed in the lastValue data member.
  // Return successFlag to indicate either success or failure.
  bool write(uint8_t value)
  {
    if (value == HIGH)
    {
      flashes++;
    }
    lastValue = value;
    return successFlag;
  }
  
  int flashes; // Current count of the number of HIGH signals since object creation
  bool successFlag; // If this is set to false, flag a failure
  uint8_t lastValue; // The last value passed to the write() method.
};

MorseCodeOutputPin morsePin(13);

// Test the encoding of the message "Hello World" against hand-encoded morse code
test(encodeHelloWorld)
{
  String text = "Hello World";
  String expected = ".... . .-.. .-.. ---   .-- --- .-. .-.. -..";
  String encoded;
  MorseCode morse(&morsePin);
  
  bool success = morse.encode(encoded, text);
  assertTrue(success);
  assertEqual(expected, encoded);
}

// Test the encoding of the message "Hello!", where '!' is an invalid character.
test(encodeHelloError)
{
  String text = "Hello!";
  String encoded;
  MorseCode morse(&morsePin);
  String expected = ".... . .-.. .-.. --- ........";

  bool success = morse.encode(encoded, text);
  assertFalse(success);
  assertEqual(expected, encoded);

  // Switch the morse class into ignore character mode

  morse.setInvalidCharBehavior(MorseCode::IgnoreChar);

  String expected2 = ".... . .-.. .-.. ---";
  bool success2 = morse.encode(encoded, text);
  assertFalse(success2);
  assertEqual(expected2, encoded);
}

// Morse code ignores the case of letters. Test that encoding a string ignores the case.
test(encodeIgnoreCase)
{
  String upperText = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,:?";
  
  // In order to avoid using lots of memory, test this 4 characters at a time
  const int snippetSize = 4;
  for (int offset = 0; offset < upperText.length(); offset += snippetSize) {
    String upperSnippet, lowerSnippet;
    
    if (offset + snippetSize > upperText.length()) { // short snippet at end of string
      upperSnippet = upperText.substring(offset); 
    } else {
      upperSnippet = upperText.substring(offset, offset + snippetSize);
    } // if (short snippet at end of string) else
    
    lowerSnippet = upperSnippet;
    lowerSnippet.toLowerCase();

    String upperMorse, lowerMorse;
    MorseCode morse(&morsePin);
    bool upperSuccess = morse.encode(upperMorse, upperSnippet);
    bool lowerSuccess = morse.encode(lowerMorse, lowerSnippet);
    assertTrue(upperSuccess);
    assertTrue(lowerSuccess);
    assertEqual(lowerMorse, upperMorse);
  } // for offset
}

// Test the encoding of the letters A-M against hand-coded morse code
test(encodeAtoM)
{
  String letters = "ABCDEFGHIJKLM";
  String expected = ".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. --";
  String encoded;
  MorseCode morse(&morsePin);
  
  bool success = morse.encode(encoded, letters);
  assertTrue(success);
  assertEqual(expected, encoded);  
}

// Test the encoding of the letters N-Z against hand-coded morse code
test(encodeNtoZ)
{
  String letters = "NOPQRSTUVWXYZ";
  String expected = "-. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --..";
  String encoded;
  MorseCode morse(&morsePin);
  
  bool success = morse.encode(encoded, letters);
  assertTrue(success);
  assertEqual(expected, encoded);  
}

// Test the encoding of punctuation
test(encodePunctuation)
{
  String punc1 = ".,:?'-/()\"";
  String expected1 = ".-.-.- --..-- ---... ..--.. .----. -....- -..-. -.--. -.--.- .-..-.";
  String encoded1;
  MorseCode morse(&morsePin);
  
  bool success1 = morse.encode(encoded1, punc1);
  assertTrue(success1);
  assertEqual(expected1, encoded1);
  
  String punc2 = "=+*@";
  String expected2 = "-...- .-.-. -..- .--.-.";
  String encoded2;
  
  bool success2 = morse.encode(encoded2, punc2);
  assertTrue(success2);
  assertEqual(expected2, encoded2);
}

// Test the encoding of digits
//
// The morse code for a digit is a sequence of five dots or dashes.
// The digits 1-5 start with a number of dots equal to the digit.
// These is then padded to 5 with dashes.
// The digits 6-9 start with a number of dashes equal to the digit minus five.
// These are padded to 5 with dots.
// The digit 0 is five dashes. 
test(encodeDigit)
{
  for (int digit = 0; digit <= 9; digit++) {
    String expected = "";
    
    if (digit == 0) {
      expected = "-----";
    } else { // 1-9
      if (digit <= 5) { // 1-5 start with dots
        int dots;
        
        for (dots = 0; dots < digit ; dots++) {
          expected += ".";
        } // for dots
        
        for (int padding = dots; padding < 5; padding++) {
          expected += "-";
        } // for padding
      } else { // 6-9 start with dashes
        int dashes;
        
        for (dashes = 0; dashes < (digit - 5) ; dashes++) {
          expected += "-";
        } // for dashes
        
        for (int padding = dashes; padding < 5; padding++) {
          expected += ".";
        } // for padding
      } // if (digit <=5) else
    } // if (digit == 0) else
    
    String message = String(digit);
    String encoded;
    MorseCode morse(&morsePin);
    
    bool success = morse.encode(encoded, message);
    assertTrue(success);
    assertEqual(expected, encoded);
  } // for digit
}

// Count the number of flashes output by the code "--. .".
test(sendCodeCountFlashes)
{
  String code = "--. .";
  int expectedFlashes = 4;
  uint8_t expectedLastValue = LOW;
  MockMorseCodeOutput mockOutput;
  MorseCode morse(&mockOutput);

  bool success = morse.sendCode(code);
  assertTrue(success);
  assertEqual(expectedFlashes, mockOutput.flashes);
  assertEqual(expectedLastValue, mockOutput.lastValue);
}

// Verify that when calling MorseCode::sendCode(), a success from the output object
// results in success, and an error from the output object results in failure.
test(sendCodeOutputFailure)
{
  String code = "..";
  MockMorseCodeOutput mockOutput;
  MorseCode morse(&mockOutput);

  bool success = morse.sendCode(code);
  assertTrue(success);
  
  mockOutput.successFlag = false;

  success = morse.sendCode(code);
  assertFalse(success);
}

// Check that an invalid character causes MorseCode::write() to fail.
test(writeHelloError)
{
  String message = "Hello!";

  MockMorseCodeOutput mockOutput;
  MorseCode morse(&mockOutput);

  bool success = morse.write(message);

  assertFalse(success);
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Test::run();
}
