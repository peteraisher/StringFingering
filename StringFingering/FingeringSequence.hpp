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
#include <vector>

namespace string_fingering {

class FingeringSequence {
  std::vector<uint8_t> string;
  std::vector<uint8_t> finger;
 public:
  FingeringSequence(const std::vector<uint8_t>& string_in,
                    const std::vector<uint8_t>& finger_in) {
    std::copy(string_in.begin(), string_in.end(), std::back_inserter(string));
    std::copy(finger_in.begin(), finger_in.end(), std::back_inserter(finger));
  }

  explicit FingeringSequence(size_t count) {
    string.resize(count);
    finger.resize(count);
  }

  inline void setString(size_t index, uint8_t value) {
    string[index] = value;
  }
  inline void setFinger(size_t index, uint8_t value) {
    finger[index] = value;
  }

  inline uint8_t getString(size_t index) {return string[index];}
  inline uint8_t getFinger(size_t index) {return finger[index];}
  inline size_t getCount() {return std::max(string.size(), finger.size());}
};

}   // namespace string_fingering

#endif /* FingeringSequence_hpp */
