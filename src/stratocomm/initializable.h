#ifndef RCR_INITIALIZABLE_H
#define RCR_INITIALIZABLE_H

namespace rcr {

class Initializable {
 public:
  Initializable(const char* display_name);

  virtual const char* display_name() const;

  // Returns true if initialization is successful.
  virtual bool Init() = 0;

  virtual bool IsFullyInitialized() const;

  virtual ~Initializable();

 protected:
  const char* display_name_;
  bool init_result_ = false;
};

} // namespace rcr

#endif // RCR_INITIALIZABLE_H
