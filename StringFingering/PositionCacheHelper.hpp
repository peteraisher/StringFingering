//  Copyright (c) 2022 Peter Aisher
//
//  PositionCacheHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef STRINGFINGERING_POSITIONCACHEHELPER_HPP_
#define STRINGFINGERING_POSITIONCACHEHELPER_HPP_

#include <cstddef>
#include "../StringFingering/types.h"

namespace string_fingering {

class PositionCacheHelper {
  position_t* storage = nullptr;
  size_t currentIndex = 0;
  size_t stringCount = 0;
 public:
  explicit PositionCacheHelper(size_t stringCount);
  ~PositionCacheHelper();
  void next();
  position_t& current(size_t onString);
  position_t& previous(size_t onString);
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_POSITIONCACHEHELPER_HPP_
