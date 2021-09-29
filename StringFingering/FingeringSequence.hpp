//
//  FingeringSequence.hpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#ifndef FingeringSequence_hpp
#define FingeringSequence_hpp

#include <cstdint>
#include <cstring>

class FingeringSequence {
 public:
  FingeringSequence(int count, uint8_t* string_in, uint8_t* finger_in): count(count) {
    string = new uint8_t[count];
    finger = new uint8_t[count];
    if (string_in)
      memcpy(string, string_in, count);
    if (finger_in)
      memcpy(finger, finger_in, count);
  }

  ~FingeringSequence() {
    delete [] string;
    delete [] finger;
  }

  inline void setString(int index, uint8_t value) {
    string[index] = value;
  }
  inline void setFinger(int index, uint8_t value) {
    finger[index] = value;
  }

  inline uint8_t getString(int index) {return string[index];}
  inline uint8_t getFinger(int index) {return finger[index];}
  inline int getCount() {return count;}

 private:
  int count;
  uint8_t* string;
  uint8_t* finger;
};

#endif /* FingeringSequence_hpp */
