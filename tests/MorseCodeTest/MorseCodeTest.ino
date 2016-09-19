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

// Test the encoding of the message "Hello World" against hand-encoded morse code
test(encodeHelloWorld)
{
  String text = "Hello World";
  String expected = ".... . .-.. .-.. ---   .-- --- .-. .-.. -..";
  String encoded = MorseCode::encode(text);
  assertEqual(expected, encoded);
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

    String upperMorse = MorseCode::encode(upperSnippet);
    String lowerMorse = MorseCode::encode(lowerSnippet);
    assertEqual(lowerMorse, upperMorse);
  } // for offset
}

// Test the encoding of the letters A-M against hand-coded morse code
test(AtoM)
{
  String letters = "ABCDEFGHIJKLM";
  String expected = ".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. --";
  String encoded = MorseCode::encode(letters);
  assertEqual(expected, encoded);  
}

// Test the encoding of the letters N-Z against hand-coded morse code
test(NtoZ)
{
  String letters = "NOPQRSTUVWXYZ";
  String expected = "-. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --..";
  String encoded = MorseCode::encode(letters);
  assertEqual(expected, encoded);  
}

// Test the encoding of punctuation
test(punctuation)
{
  String punc1 = ".,:?'-/()\"";
  String expected1 = ".-.-.- --..-- ---... ..--.. .----. -....- -..-. -.--. -.--.- .-..-.";
  String encoded1 = MorseCode::encode(punc1);
  assertEqual(expected1, encoded1);
  
  String punc2 = "=+*@";
  String expected2 = "-...- .-.-. -..- .--.-.";
  String encoded2 = MorseCode::encode(punc2);
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
    String encoded = MorseCode::encode(message);
    assertEqual(expected, encoded);
  } // for digit
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Test::run();
}
