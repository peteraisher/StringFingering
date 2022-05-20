//
//  ScoreCacheHelper.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef ScoreCacheHelper_hpp
#define ScoreCacheHelper_hpp

#include <cstddef>
#include "types.h"

namespace string_fingering {

class ScoreCacheHelper {
  score_t* storage = nullptr;
  size_t currentIndex = 0;
  size_t stringCount = 0;
  enum {
    kFingerCount = 5
  };
 public:
  ScoreCacheHelper(size_t stringCount);
  ~ScoreCacheHelper();
  void next();
  score_t& current(size_t string, size_t finger);
  score_t& previous(size_t string, size_t finger);
};

}   // namespace string_fingering

#endif /* ScoreCacheHelper_hpp */
