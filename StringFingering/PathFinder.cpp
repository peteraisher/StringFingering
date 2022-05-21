//
//  PathFinder.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "PathFinder.hpp"

namespace string_fingering {

PathFinder::PathFinder(size_t noteCount,
                       OptimizerDelegate* delegate)
: delegate(delegate), stringCount(delegate->stringCount()),
scores(delegate->stringCount()), positions(delegate->stringCount()),
map(noteCount, delegate->stringCount()), kStringBits(0), kStringMask(1) {
  while (stringCount > kStringMask) {
    kStringMask <<= 1;
    ++kStringBits;
  }
  --kStringMask;
}

void PathFinder::setFirstPitch(uint8_t pitch) {
  for (size_t string = 0; string < stringCount; ++string) {
    position_t pos = pitch - delegate->openStringPitch(string);
    if (pos < 0) {
      positions.current(string) = -1;
    } else {
      positions.current(string) = pos;
    }
    for (int finger = 0; finger < kFingerCount; ++finger) {
      scores.current(string, finger) = delegate->rawPositionScore(pos, string, finger);
    }
  }
}

void PathFinder::markStringAsImpossibleForCurrentPitch(int s) {
  positions.current(s) = -1;
  for (int f = 0; f < kFingerCount; ++f) {
    scores.current(s, f) = delegate->never();
    map.value(index, s, f) = -1;
  }
}

void PathFinder::advanceHelperVariables() {
  scores.next();
  positions.next();
  delegate->nextNote();
  ++index;
}

void PathFinder::resetBests() {
  best_string = -1;
  best_finger = -1;
  best_score = std::numeric_limits<score_t>::max();
}

void PathFinder::evaluatePreviousFinger(int fPrev, int sPrev) {
  string_t stringDiff = string - sPrev;
  score_t fingerScore = stringScore
  + delegate->fingerChangePenalty(posPrev, pos, fPrev, finger, stringDiff)
  + scores.previous(sPrev, fPrev);
  if (fingerScore < best_score) {
    best_score = fingerScore;
    best_string = sPrev;
    best_finger = fPrev;
  }
}

void PathFinder::evaluatePreviousString(int sPrev) {
  posPrev = positions.previous(sPrev);
  if (posPrev < 0) {
    return;   // previous pitch was not playable on previous string
  }
  stringScore = delegate->stringCrossPenalty(string - sPrev) + rawScore;
  if (stringScore >= best_score) {
    return;   // all possible scores are >= stringScore so also >= best_score
  }

  for (int fPrev = 0; fPrev < kFingerCount; ++fPrev) {
    // compare position difference to finger difference
    evaluatePreviousFinger(fPrev, sPrev);
  }
}

void PathFinder::setNeverScoreIfNothingFound() {
  if (best_string == -1
      || best_finger == -1
      || best_score == std::numeric_limits<score_t>::max()) {
    best_score = delegate->never();
  }
}

void PathFinder::storeBestScoreInMap() {
  map_t prev_pointer = (best_finger << kStringBits) | best_string;
  scores.current(string, finger) = best_score;
  map.value(index, string, finger) = prev_pointer;
}

void PathFinder::findAndStoreBestTransitionToCurrentStringAndFinger() {
  rawScore = delegate->rawPositionScore(pos, string, finger);
  resetBests();
  for (int sPrev = 0; sPrev < stringCount; ++sPrev) {
    evaluatePreviousString(sPrev); // end for fPrev
  } // end for sPrev
  setNeverScoreIfNothingFound();
  storeBestScoreInMap();
}

void PathFinder::evaluateScoresForString() {
  if (pos < 0) {
    markStringAsImpossibleForCurrentPitch(string);
    return;
  }
  positions.current(string) = pos;

  for (finger = 0; finger < kFingerCount; ++finger) {
    findAndStoreBestTransitionToCurrentStringAndFinger();
  }
}

void PathFinder::appendPitch(uint8_t pitch) {
  advanceHelperVariables();
  for (string = 0; string < stringCount; ++string) {
    pos = pitch - delegate->openStringPitch(string);
    evaluateScoresForString();
  } // end for s
}

void PathFinder::findBestFingerAndStringForCurrentPitch() {
  for (int s = 0; s < stringCount; ++s) {
    for (int f = 0; f < kFingerCount; ++f) {
      if (scores.current(s, f) < best_score) {
        best_score = scores.current(s, f);
        best_string = s;
        best_finger = f;
      }
    }
  }
}

void PathFinder::readPreviousBestFingerAndStringFromMap(size_t i) {
  map_t m = map.value(i, best_string, best_finger);
  best_string = m & kStringMask;
  best_finger = m >> kStringBits;
}

void PathFinder::setFingeringFromBestAtIndex(size_t i,
                                             FingeringSequence &result) {
  result.setFinger(i, best_finger);
  result.setString(i, best_string);
}

FingeringSequence PathFinder::readOutput() {
  resetBests();
  findBestFingerAndStringForCurrentPitch();
  FingeringSequence result = FingeringSequence(index + 1);
  for (size_t i = index; i > 0; --i) {
    setFingeringFromBestAtIndex(i, result);
    readPreviousBestFingerAndStringFromMap(i);
  }
  setFingeringFromBestAtIndex(0, result);
  return result;
}

}   // namespace string_fingering
