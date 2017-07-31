#ifndef RCR_STRATOCOMM_OCTETARRAY_H_
#define RCR_STRATOCOMM_OCTETARRAY_H_

#include "constants.h"

namespace rcr {
namespace stratocomm {
namespace octet_array {

// Get an octet array of the string content (including the null
// terminator '\0') along with its length.
constexpr uint8_t get_octets(const String& text, uint8_t* octets_buffer) {
  auto length = text.length(); // without terminator

  for (auto i = 0u; i < length; ++i) {
    octets_buffer[i] = static_cast<uint8_t>(text.charAt(i));
  }
  octets_buffer[length++] = '\0'; // add terminator

  return static_cast<uint8_t>(length);
};

// Get a String from an octet array and its length. Trim either side as needed.
constexpr void get_string(const uint8_t* const octets, uint8_t length, char* buffer,
    uint8_t forward_offset = static_cast<uint8_t>(0),
    uint8_t offset_from_back = static_cast<uint8_t>(0)) {
  const uint8_t* const trimmed_octets = &(octets[forward_offset]);
  const uint8_t trimmed_length = length - forward_offset - offset_from_back;
  if (trimmed_length <= 0) {
    buffer[0] = '\0';
    return;
  }
  for (uint8_t i = 0; i < trimmed_length; ++i) {
    buffer[i] = static_cast<char>(trimmed_octets[i]);
  }
  buffer[trimmed_length] = '\0'; // ensure strlen() will return correct length.
  return;
};

} // namespace octet_array
} // namespace stratocomm
} // namespace rcr

#endif // RCR_STRATOCOMM_OCTETARRAY_H_
