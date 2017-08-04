#ifndef RCR_STRATOCOMM_STRATOCOMMSENDSPEEDTEST_H_
#define RCR_STRATOCOMM_STRATOCOMMSENDSPEEDTEST_H_

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
  constexpr int kTotal = 100;
  Radio radio(Serial1);
  auto count = 0; // allow a buffer for radios to start working
  uint32_t start_time;
  auto time_intervals = std::vector<uint32_t>(kTotal);
} //  namespace

std::vector<Initializable*> components{ &radio };
std::vector<Updateable*> updateables{ &radio };

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

  Serial.println("Setup complete; starting send-time test.");
  radio.Send("foo");
  radio.Send("foo");
  radio.Send("foo");
  radio.Send("foo");
  radio.Send("foo");
  delay(3000);
}

inline void loop() {
  if (count++ == kTotal) {
    auto sum = 0l;

    // Show mean send-time.
    for (auto& interval : time_intervals) {
      sum += interval;
      if (sum < 0l) {
        Serial.println("addition OVERFLOW: result accuracy is compromised");
      }
    }
    auto mean = sum / time_intervals.size();

    Serial.print("Mean send time is: ");
    Serial.println(mean);

    // Stop forever.
    for (;;) {}
  }

  start_time = millis();
  radio.Send("qn6iacxzd1qxnl8j7adwbzdgdxqvgmodaxeblsmhm4638tioozqn6iacxzd1qxnl8j7adwbzdgdxqvgmodaxeblsmhm4638tiooz");
  auto total_time = millis() - start_time;


  time_intervals.push_back(total_time);
  Serial.println(total_time);

  // Update everything.
  for (auto& u : updateables) {
    u->Update();
  }

  blink();
}

} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_STRATOCOMMSENDSPEEDTEST_H_
