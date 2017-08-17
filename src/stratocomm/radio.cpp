#include "radio.h"

#include "constants.h"

namespace rcr {
namespace stratocomm {

namespace {
  // DANGER: this design effectively makes Radio Singleton.
  // TODO: redsign to allow multiple radios
  Radio* radio_instance; // ok, because we only use one.

  // Surrogate callback, submitted to XBee library, to call Radio member function.
  void on_receive(XBeeResponse& incoming_transmission, uintptr_t) {
    radio_instance->OnReceive(incoming_transmission);
  }

  // Address which XBee interperets as a broadcast.
  constexpr uint32_t kXbeeBroadcastAddressMsb = 0x00000000;
  constexpr uint32_t kXbeeBroadcastAddressLsb = 0x0000ffff;

  // The XBee's interal baud rate for communication with its commanding module.
  // (This must exactly match the (software configured) XBee baud rate.)
  constexpr uint32_t kXbeeSerialInterfaceBaudRate = 230400;
} // namespace

Radio::Radio(HardwareSerial & serial)
    : Initializable("Radio"), radio_serial_(serial) {
  addr64_.setMsb(kXbeeBroadcastAddressMsb);
  addr64_.setLsb(kXbeeBroadcastAddressLsb);
}

bool Radio::Init() {
  // Setup serial comm.
  radio_serial_.begin(kXbeeSerialInterfaceBaudRate);
  xbee_.setSerial(radio_serial_);

  // Setup packet addressing.
  outbound_packet_.setAddress64(addr64_);

  // Set up callback on incoming transmission.
  radio_instance = this;
  xbee_.onOtherResponse(on_receive);

  init_result_ = true;
  return init_result_;
}

void Radio::Send(const String& text) {
  auto length = text.length(); // without terminator

  for (auto i = 0u; i < length; ++i) {
    octets_buffer_[i] = static_cast<uint8_t>(text.charAt(i));
  }

  outbound_packet_.setPayload(octets_buffer_, length);
  xbee_.send(outbound_packet_);
  Serial.println("sent");
}

inline void Radio::OnReceive(XBeeResponse& incoming_transmission) {
  ZBRxResponse r;
  incoming_transmission.getZBRxResponse(r);

  auto get_string = [](const uint8_t* const octets, uint8_t length,
    uint8_t forward_offset = static_cast<uint8_t>(0),
    uint8_t offset_from_back = static_cast<uint8_t>(0)) -> String {
    // [kMaxPayloadLength + 1] to include null terminator '\0'
    char buffer[kMaxPayloadLength + 1];

    const uint8_t* const trimmed_octets = &(octets[forward_offset]);

    // if new length is < 1 return empty string.
    if (length <= forward_offset + offset_from_back) {
      auto empty_string = String("");
      return empty_string;
    }

    const uint8_t trimmed_length = length - forward_offset - offset_from_back;
    for (uint8_t i = 0; i < trimmed_length; ++i) {
      buffer[i] = static_cast<char>(trimmed_octets[i]);
    }
    buffer[trimmed_length] = '\0'; // ensure strlen() will return correct length.

    // Convert to string.
    auto s = String(buffer);
    return s;
  };

  auto transmission = get_string(r.getData(), r.getDataLength());
  strcpy(char_buffer_, transmission.c_str());
  has_new_message_ = true;
}

const char* Radio::last_message() {
  has_new_message_ = false;
  return char_buffer_;
}

bool Radio::HasUnreadMessage() {
  return has_new_message_;
}

void Radio::Update() {
  xbee_.loop();
}

} // namespace stratocomm
} // namespace rcr
