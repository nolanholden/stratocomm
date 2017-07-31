#ifndef RCR_STRATOCOMM_STRATOCOMM_H_
#define RCR_STRATOCOMM_STRATOCOMM_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "constants.h"
#include "initializable.h"
#include "radio.h"
#include "updateable.h"

#include <vector>

namespace rcr {
namespace stratocomm {

Radio sender(Serial1);
Radio receiver(Serial2);

std::vector<Initializable*> components{ &sender, &receiver };
std::vector<Updateable*> updateables{ &sender, &receiver };

inline void blink() {
  digitalWrite(kLedPin, HIGH);
  digitalWrite(kLedPin, LOW);
}

inline void setup() {
  // Wait a moment.
  delay(1024u);

  // Start serial communication.
  Serial.begin(9600l); // baud does not matter for Teensy 3.6

  // Set LED pin.
  pinMode(kLedPin, OUTPUT);

  // Initialize components.
  for (auto& c : components) {
    if (!c->Init()) {
      Serial.print(c->display_name());
      Serial.println(" initialization failed!");
    }
  }

  Serial.println("Setup complete; entering loop.");
}

inline void loop() {
  // Send something arbitrary.
  sender.Send("hey, world!");

  // Update everything.
  for (auto& u : updateables) {
    u->Update();
  }

  // If we got a new message, show us.
  if (receiver.HasUnreadMessage()) {
    Serial.println(receiver.last_message());
  }

  blink();
}

} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_STRATOCOMM_H_
