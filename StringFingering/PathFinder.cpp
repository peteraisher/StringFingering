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

void PathFinder::appendPitch(uint8_t pitch) {
  advanceHelperVariables();

  for (int s = 0; s < stringCount; ++s) {
    position_t pos = pitch - delegate->openStringPitch(s);
    if (pos < 0) {
      markStringAsImpossibleForCurrentPitch(s);
      continue;
    }
    positions.current(s) = pos;

    for (int f = 0; f < kFingerCount; ++f) {
      score_t rawScore = delegate->rawPositionScore(pos, s, f);
      string_t best_string = -1;
      finger_t best_finger = -1;
      score_t best_score = std::numeric_limits<score_t>::max();
      for (int sPrev = 0; sPrev < stringCount; ++sPrev) {
        position_t posPrev = positions.previous(sPrev);
        if (posPrev < 0) {
          continue;
        }
        score_t stringScore = delegate->stringCrossPenalty(s - sPrev) + rawScore;
        if (stringScore > best_score) {
          continue;
        }
        for (int fPrev = 0; fPrev < kFingerCount; ++fPrev) {
          // compare position difference to finger difference
          string_t stringDiff = s - sPrev;
          score_t fingerScore = stringScore
            + delegate->fingerChangePenalty(posPrev, pos, fPrev, f, stringDiff)
            + scores.previous(sPrev, fPrev);
          if (fingerScore < best_score) {
            best_score = fingerScore;
            best_string = sPrev;
            best_finger = fPrev;
          }
        } // end for fPrev
      } // end for sPrev
      if (best_string == -1
          || best_finger == -1
          || best_score == std::numeric_limits<score_t>::max()) {
        best_score = delegate->never();
      }
      map_t prev_pointer = (best_finger << kStringBits) | best_string;
      scores.current(s, f) = best_score;
      map.value(index, s, f) = prev_pointer;
    } // end for f
  } // end for s
}

FingeringSequence PathFinder::readOutput() {
  string_t best_string = -1;
  finger_t best_finger = -1;
  score_t best_score = std::numeric_limits<score_t>::max();
  for (int s = 0; s < stringCount; ++s) {
    for (int f = 0; f < kFingerCount; ++f) {
      if (scores.current(s, f) < best_score) {
        best_score = scores.current(s, f);
        best_string = s;
        best_finger = f;
      }
    }
  }
  FingeringSequence result = FingeringSequence(index + 1);
  for (size_t i = index; i > 0; --i) {
    result.setFinger(i, best_finger);
    result.setString(i, best_string);
    map_t m = map.value(i, best_string, best_finger);
    best_string = m & kStringMask;
    best_finger = m >> kStringBits;
  }
  result.setFinger(0, best_finger);
  result.setString(0, best_string);
  return result;
}

}   // namespace string_fingering
