//
//  PositionCacheHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef PositionCacheHelper_hpp
#define PositionCacheHelper_hpp

#include <cstddef>
#include "types.h"

namespace string_fingering {

class PositionCacheHelper {
  position_t* storage = nullptr;
  size_t currentIndex = 0;
  size_t stringCount = 0;
 public:
  PositionCacheHelper(size_t stringCount);
  ~PositionCacheHelper();
  void next();
  position_t& current(size_t string);
  position_t& previous(size_t string);
};

}   // namespace string_fingering

#endif /* PositionCacheHelper_hpp */
