//  Copyright (c) 2022 Peter Aisher
//
//  MapHelper.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "../StringFingering/MapHelper.hpp"

namespace string_fingering {

namespace impl {

void MapHelper::fillFirstIndexWithInvalid(size_t stringCount) {
  for (size_t s = 0; s < stringCount; ++s) {
    for (size_t f = 0; f < stringCount; ++f) {
      value(0, s, f) = -1;
    }
  }
}

MapHelper::MapHelper(size_t noteCount, size_t stringCount)
: stringCount(stringCount) {
  storage = new map_t[noteCount * stringCount * kFingerCount]();
  fillFirstIndexWithInvalid(stringCount);
}
MapHelper::~MapHelper() {
  delete [] storage;
}
map_t& MapHelper::value(size_t index, size_t string, size_t finger) {
  return storage[((index * stringCount) + string) * kFingerCount + finger];
}

}   // namespace impl

}   // namespace string_fingering
