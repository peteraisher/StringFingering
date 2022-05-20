//
//  MapHelper.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "MapHelper.hpp"

namespace string_fingering {

MapHelper::MapHelper(size_t noteCount, size_t stringCount)
: stringCount(stringCount) {
  storage = new map_t[noteCount * stringCount * kFingerCount]();
}
MapHelper::~MapHelper() {
  delete [] storage;
}
map_t& MapHelper::value(size_t index, size_t string, size_t finger) {
  return storage[((index * stringCount) + string) * kFingerCount + finger];
}

}   // namespace string_fingering
