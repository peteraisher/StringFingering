//  Copyright (c) 2022 Peter Aisher
//
//  ScoreCacheHelper.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "../StringFingering/ScoreCacheHelper.hpp"

namespace string_fingering {

ScoreCacheHelper::ScoreCacheHelper(size_t stringCount)
: stringCount(stringCount) {
  storage = new score_t[stringCount * kFingerCount * 2]();
}
ScoreCacheHelper::~ScoreCacheHelper() {
  delete [] storage;
}
score_t& ScoreCacheHelper::current(size_t string, size_t finger) {
  return storage[(currentIndex * stringCount + string) * kFingerCount + finger];
}

score_t& ScoreCacheHelper::previous(size_t string, size_t finger) {
  size_t prevIndex = currentIndex ^ 1;
  return storage[(prevIndex * stringCount + string) * kFingerCount + finger];
}

void ScoreCacheHelper::next() {currentIndex ^= 1;}

}   // namespace string_fingering
