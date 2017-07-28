#ifndef RCR_STRATOCOMM_STRATOCOMM_H_
#define RCR_STRATOCOMM_STRATOCOMM_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "constants.h"

namespace rcr {
namespace stratocomm {

namespace {
  inline void blink() {
    digitalWrite(kLedPin, HIGH);
    digitalWrite(kLedPin, LOW);
  }
} // namespace

inline void setup() {
  // Wait a moment.
  delay(1024u);

  // Set LED pin.
  pinMode(kLedPin, OUTPUT);

  // Start serial communication.
  Serial.begin(9600l); // baud does not matter for Teensy 3.6

  Serial.println("Setup complete; entering loop.");
}

inline void loop() {

  blink();
}

} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_STRATOCOMM_H_
