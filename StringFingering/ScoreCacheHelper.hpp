//  Copyright (c) 2022 Peter Aisher
//
//  ScoreCacheHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef STRINGFINGERING_SCORECACHEHELPER_HPP_
#define STRINGFINGERING_SCORECACHEHELPER_HPP_

#include <cstddef>
#include "../StringFingering/types.h"

namespace string_fingering {

class ScoreCacheHelper {
  score_t* storage = nullptr;
  size_t currentIndex = 0;
  size_t stringCount = 0;
  enum {
    kFingerCount = 5
  };
 public:
  explicit ScoreCacheHelper(size_t stringCount);
  ~ScoreCacheHelper();
  void next();
  score_t& current(size_t onString, size_t finger);
  score_t& previous(size_t onString, size_t finger);
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_SCORECACHEHELPER_HPP_
