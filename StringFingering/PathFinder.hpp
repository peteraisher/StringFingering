//  Copyright (c) 2022 Peter Aisher
//
//  PathFinder.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef STRINGFINGERING_PATHFINDER_HPP_
#define STRINGFINGERING_PATHFINDER_HPP_

#include <cstddef>
#include "../StringFingering/OptimizerDelegate.hpp"
#include "../StringFingering/MapHelper.hpp"
#include "../StringFingering/PositionCacheHelper.hpp"
#include "../StringFingering/ScoreCacheHelper.hpp"
#include "../StringFingering/FingeringSequence.hpp"

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
  score_t rawScore = 0;
  score_t stringScore = 0;
  score_t best_score = 0;
  position_t pos = 0;
  position_t posPrev = 0;
  string_t best_string = 0;
  finger_t best_finger = 0;
  int currentString = 0;
  int currentFinger = 0;

  enum {
    kFingerCount = 5
  };
  void markStringAsImpossibleForCurrentPitch(int s);
  void advanceHelperVariables();
  void resetBests();
  void evaluatePreviousFingerAndUpdateBest(int fPrev, int sPrev);
  void evaluatePreviousStringAndUpdateBest(int sPrev);
  void setNeverScoreIfNothingFound();
  void storeBestScoreInMap();
  void findAndStoreBestTransitionToCurrentStringAndFinger();
  void evaluateAndStoreScoresForString();
  void findBestFingerAndStringForCurrentPitch();
  void readPreviousBestFingerAndStringFromMap(size_t i);
  void setFingeringFromBestAtIndex(size_t i, FingeringSequence* result);

 public:
  PathFinder(size_t noteCount, OptimizerDelegate* delegate);
  void setFirstPitch(uint8_t pitch);
  void appendPitch(uint8_t pitch);
  FingeringSequence readOutput();
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_PATHFINDER_HPP_
