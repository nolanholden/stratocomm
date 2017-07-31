#ifndef RCR_UPDATEABLE_H
#define RCR_UPDATEABLE_H

namespace rcr {

class Updateable {
 public:
  // Do this every time in loop();
  virtual void Update() = 0;

  virtual ~Updateable();
};

} // namespace rcr

#endif // RCR_UPDATEABLE_H
