//  Copyright (c) 2022 Peter Aisher
//
//  MapHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef STRINGFINGERING_MAPHELPER_HPP_
#define STRINGFINGERING_MAPHELPER_HPP_

#include <cstddef>
#include "../StringFingering/types.h"

namespace string_fingering {

namespace impl {

class MapHelper {
  map_t* storage = nullptr;
  size_t stringCount;
  enum {
    kFingerCount = 5
  };
  void fillFirstIndexWithInvalid(size_t stringCount);

 public:
  MapHelper(size_t noteCount, size_t stringCount);
  ~MapHelper();
  map_t& value(size_t index, size_t onString, size_t finger);
};

}   // namespace impl

}   // namespace string_fingering

#endif  // STRINGFINGERING_MAPHELPER_HPP_
