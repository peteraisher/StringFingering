//
//  PathFinder.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef PathFinder_hpp
#define PathFinder_hpp

#include <cstddef>
#include "Optimizer.hpp"
#include "MapHelper.hpp"
#include "PositionCacheHelper.hpp"
#include "ScoreCacheHelper.hpp"
#include "FingeringSequence.hpp"

namespace string_fingering {

class PathFinder {
  ScoreCacheHelper scores;
  PositionCacheHelper positions;
  MapHelper map;
  OptimizerDelegate* delegate = nullptr;
  size_t index = 0;
  size_t stringCount = 0;
  size_t kStringBits = 0;
  size_t kStringMask = 1;
  enum {
    kFingerCount = 5
  };
public:
  PathFinder(size_t noteCount, OptimizerDelegate* delegate);
  void setFirstPitch(uint8_t pitch);
  void appendPitch(uint8_t pitch);
  FingeringSequence readOutput();
};

}   // namespace string_fingering

#endif /* PathFinder_hpp */
