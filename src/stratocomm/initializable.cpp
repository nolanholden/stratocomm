#include "initializable.h"

namespace rcr {

Initializable::Initializable(const char* display_name)
  : display_name_(display_name) {}

const char* Initializable::display_name() const { return display_name_; }

bool Initializable::IsFullyInitialized() const { return init_result_; }

Initializable::~Initializable() {}

} // namespace rcr
