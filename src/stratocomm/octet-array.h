#ifndef RCR_STRATOCOMM_OCTETARRAY_H_
#define RCR_STRATOCOMM_OCTETARRAY_H_

#include "constants.h"

#include <utility>

namespace rcr {
namespace stratocomm {
namespace octets {

namespace {
  // static duration buffer (to prevent allocation upon every call.)
  // [kMaxPayloadLength + 1] to include null terminator '\0'
  char static_char_buffer[kMaxPayloadLength + 1];

  // Garuntee array (pointer) liftime for get_octets();.
  uint8_t static_octets_buffer[kMaxPayloadLength];
} // namespace

String get_string(const uint8_t* const octets, uint8_t length,
    uint8_t forward_offset = static_cast<uint8_t>(0),
    uint8_t offset_from_back = static_cast<uint8_t>(0)) {
  const uint8_t* const trimmed_octets = &(octets[forward_offset]);
  const uint8_t trimmed_length = length - forward_offset - offset_from_back;
  if (trimmed_length <= 0) {
    auto s = String("");
    return s;
  }
  for (uint8_t i = 0; i < trimmed_length; ++i) {
    static_char_buffer[i] = static_cast<char>(trimmed_octets[i]);
  }
  static_char_buffer[trimmed_length] = '\0'; // ensure strlen() will return correct length.
  auto s = String(static_char_buffer);
  return s;
}

// Get an octet array of the string content (not including the null
// terminator '\0') along with its length.
// CATION: Garuntees array lifetime only until called again.
std::pair<uint8_t*, uint8_t> get_octets(const String& text) {
  auto length = text.length();
  for (auto i = 0u; i < length; ++i) {
    static_octets_buffer[i] = static_cast<uint8_t>(text.charAt(i));
  }
  auto pair = std::make_pair(static_octets_buffer, length);
  return pair;
}

} // namespace octets
} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_OCTETARRAY_H_
