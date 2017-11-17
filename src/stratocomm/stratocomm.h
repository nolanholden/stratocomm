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

namespace {
  Radio radio(Serial1);
  std::vector<Initializable*> components{ &radio };
  std::vector<Updateable*> updateables{ &radio };

  inline void blink() {
    digitalWrite(kLedPin, HIGH);
    digitalWrite(kLedPin, LOW);
  }

  constexpr const char* const ninetysix_octet_string = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345";
  String line = ninetysix_octet_string;
  String last_message;
  int last_starting_index;
  uint32_t last_time;
  int count = 0;
} //  namespace


inline void setup() {
  // Wait a moment.
  delay(1024u);

  // Start serial communication.
  Serial.begin(2000000l); // baud does not matter for Teensy 3.6

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
  // Send. (Comment out for receiveing module.)
  delay(90);
  radio.Send(line);
  line.setCharAt(count, '-');
  if (++count == line.length()) {
    line = ninetysix_octet_string;
    count = 0;
  }

  // Update everything.
  for (auto& u : updateables) {
    u->Update();
  }

  // Receive.
  // If we have a new message, show us.
  if (radio.HasUnreadMessage()) {
    Serial.print("Message is: ");
    String new_message = radio.last_message();
    Serial.print(new_message);

    Serial.print("     Time [ms] since last received transmission: ");
    Serial.println(millis() - last_time);
    last_time = millis();

    auto new_starting_index = 0;
    while (new_starting_index < last_message.length() && new_message[new_starting_index] == '-') {
      ++new_starting_index;
    }
    if (new_starting_index != last_starting_index + 1) {
      if (new_starting_index != 0) { // ignores "wrapping" case
        Serial.println("  ---> NOTE: SKIPPED ONE MESSAGE");
      }
    }
    // update
    last_message = new_message;
    last_starting_index = new_starting_index;
  }


  blink();
}

} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_STRATOCOMM_H_
