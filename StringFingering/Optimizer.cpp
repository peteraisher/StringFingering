//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.cpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#include "Optimizer.hpp"

namespace string_fingering {

Optimizer::Optimizer(OptimizerDelegate* delegate)
: delegate(delegate), kStringBits(0), kStringMask(1) {
  int stringCount = delegate->stringCount();
  while (stringCount > kStringMask) {
    kStringMask <<= 1;
    ++kStringBits;
  }
  --kStringMask;
}

FingeringSequence Optimizer::calculate(const SingleNoteSequence& sequence) {

  auto pitches = sequence.getPitches();
  auto count = sequence.getCount();

  int kStringCount = delegate->stringCount();

  auto scores = new score_t [kStringCount][2][kFingerCount]();
  auto positions = new position_t [kStringCount][2]();
  auto map = new map_t [count*kStringCount][kFingerCount];

  uint8_t pitch = pitches[0];

  // first map elements are -1, no previous information
  for (int s = 0; s < kStringCount; ++s) {
    for (int f = 0; f < kStringCount; ++f) {
      map[0*kStringCount + s][f] = -1;
    }
  }

  // set raw scores for first notes
  for (int string = 0; string < kStringCount; ++string) {
    position_t pos = pitch - delegate->openStringPitch(string);
    if (pos < 0) {
      positions[string][0] = -1;
      for (int f = 0; f < kFingerCount; ++f) {
        scores[string][0][f] = kPenaltyNever;
      }
      continue;
    }
    positions[string][0] = pos;
    for (int finger = 0; finger < 5; ++finger) {
      scores[string][0][finger] = delegate->rawPositionScore(pos, string, finger);
    }
    if (pos) {
      scores[string][0][0] = kPenaltyNever;
    }
  }

  for (int i = 1; i < count; ++i) {

    delegate->nextNote();
    int current = i & 1;
    int prev = current ^ 1;

    pitch = pitches[i];

    for (int s = 0; s < kStringCount; ++s) {
      position_t pos = pitch - delegate->openStringPitch(s);
      if (pos < 0) {
        positions[s][current] = -1;
        for (int f = 0; f < kFingerCount; ++f) {
          scores[s][current][f] = kPenaltyNever;
          map[i * kStringCount + s][f] = -1;
        }
        continue;
      }
      positions[s][current] = pos;

      for (int f = 0; f < kFingerCount; ++f) {
        score_t rawScore = delegate->rawPositionScore(pos, s, f);
        string_t best_string = -1;
        finger_t best_finger = -1;
        score_t best_score = kPenaltyMax;
        for (int sPrev = 0; sPrev < kStringCount; ++sPrev) {
          position_t posPrev = positions[sPrev][prev];
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
              + scores[sPrev][prev][fPrev];
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
        scores[s][current][f] = best_score;
        map[i * kStringCount + s][f] = prev_pointer;
      } // end for f
    } // end for s

  } // end for i

  // now have map complete and can trace back to find best set of strings and fingerings
  string_t best_string = -1;
  finger_t best_finger = -1;
  score_t best_score = kPenaltyMax;
  int last = (count & 1) ^ 1;
  for (int s = 0; s < kStringCount; ++s) {
    for (int f = 0; f < kFingerCount; ++f) {
      if (scores[s][last][f] < best_score) {
        best_score = scores[s][last][f];
        best_string = s;
        best_finger = f;
      }
    }
  }
  FingeringSequence result = FingeringSequence(count);
  for (size_t i = count - 1; i > 0; --i) {
    result.setFinger(i, best_finger);
    result.setString(i, best_string);
    map_t m = map[i * kStringCount + best_string][best_finger];
    best_string = m & kStringMask;
    best_finger = m >> kStringBits;
  }
  result.setFinger(0, best_finger);
  result.setString(0, best_string);
  return result;
}

}   // namespace string_fingering
