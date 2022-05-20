//
//  MapHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef MapHelper_hpp
#define MapHelper_hpp

#include <cstddef>
#include "types.h"

namespace string_fingering {

class MapHelper {
  map_t* storage = nullptr;
  size_t stringCount;
  enum {
    kFingerCount = 5
  };
 public:
  MapHelper(size_t noteCount, size_t stringCount);
  ~MapHelper();
  map_t& value(size_t index, size_t string, size_t finger);
};

}   // namespace string_fingering

#endif /* MapHelper_hpp */
