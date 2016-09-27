// Hello Morse
//
// Output Hello World in morse code on the builtin LED.

#include "MorseCode.h"

MorseCodeOutputPin morsePin(13);
MorseCode morse(&morsePin);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  morse.write("Hello World");
  delay(3000);
}
