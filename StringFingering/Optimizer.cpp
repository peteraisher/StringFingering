//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.cpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#include "Optimizer.hpp"
#include "ScoreCacheHelper.hpp"
#include "PositionCacheHelper.hpp"
#include "MapHelper.hpp"
#include <vector>

namespace string_fingering {

Optimizer::Optimizer(OptimizerDelegate* delegate)
: delegate(delegate), kStringBits(0), kStringMask(1) {
  kStringCount = delegate->stringCount();
  while (kStringCount > kStringMask) {
    kStringMask <<= 1;
    ++kStringBits;
  }
  --kStringMask;
}

FingeringSequence Optimizer::calculate(const SingleNoteSequence& sequence) {

  auto pitches = sequence.getPitches();
  auto count = sequence.getCount();

  auto scores = ScoreCacheHelper(kStringCount);
  auto positions = PositionCacheHelper(kStringCount);
  auto map = MapHelper(count, kStringCount);

  uint8_t pitch = pitches[0];

  // first map elements are -1, no previous information
  for (int s = 0; s < kStringCount; ++s) {
    for (int f = 0; f < kStringCount; ++f) {
      map.value(0, s, f) = -1;
    }
  }

  // set raw scores for first notes
  for (int string = 0; string < kStringCount; ++string) {
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

  for (int i = 1; i < count; ++i) {
    scores.next();
    positions.next();
    delegate->nextNote();

    pitch = pitches[i];

    for (int s = 0; s < kStringCount; ++s) {
      position_t pos = pitch - delegate->openStringPitch(s);
      if (pos < 0) {
        positions.current(s) = -1;
        for (int f = 0; f < kFingerCount; ++f) {
          scores.current(s, f) = kPenaltyNever;
          map.value(i, s, f) = -1;
        }
        continue;
      }
      positions.current(s) = pos;

      for (int f = 0; f < kFingerCount; ++f) {
        score_t rawScore = delegate->rawPositionScore(pos, s, f);
        string_t best_string = -1;
        finger_t best_finger = -1;
        score_t best_score = kPenaltyMax;
        for (int sPrev = 0; sPrev < kStringCount; ++sPrev) {
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
            || best_score == kPenaltyMax) {
          best_score = kPenaltyNever;
        }
        map_t prev_pointer = (best_finger << kStringBits) | best_string;
        scores.current(s, f) = best_score;
        map.value(i, s, f) = prev_pointer;
      } // end for f
    } // end for s

  } // end for i

  // now have map complete and can trace back to find best set of strings and fingerings
  string_t best_string = -1;
  finger_t best_finger = -1;
  score_t best_score = kPenaltyMax;
  for (int s = 0; s < kStringCount; ++s) {
    for (int f = 0; f < kFingerCount; ++f) {
      if (scores.current(s, f) < best_score) {
        best_score = scores.current(s, f);
        best_string = s;
        best_finger = f;
      }
    }
  }
  FingeringSequence result = FingeringSequence(count);
  for (size_t i = count - 1; i > 0; --i) {
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
