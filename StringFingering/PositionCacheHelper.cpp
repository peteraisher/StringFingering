//  Copyright (c) 2022 Peter Aisher
//
//  PositionCacheHelper.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "../StringFingering/PositionCacheHelper.hpp"

namespace string_fingering {

PositionCacheHelper::PositionCacheHelper(size_t stringCount)
: stringCount(stringCount) {
  storage = new position_t[stringCount * 2]();
}
PositionCacheHelper::~PositionCacheHelper() {
  delete [] storage;
}
position_t& PositionCacheHelper::current(size_t string) {
  return storage[currentIndex * stringCount + string];
}

position_t& PositionCacheHelper::previous(size_t string) {
  size_t prevIndex = currentIndex ^ 1;
  return storage[prevIndex * stringCount + string];
}

void PositionCacheHelper::next() {currentIndex ^= 1;}

}   // namespace string_fingering
