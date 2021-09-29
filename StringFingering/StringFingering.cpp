//
//  StringFingering.cpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#include "StringFingering.hpp"
#include <cstdint>
#include <stdio.h>


FingeringSequence StringFingering::optimalFingeringFor(SingleNoteSequence noteSequence) {
  int count = noteSequence.getCount();

  uint8_t* pitches = noteSequence.getPitches();

  score_t scores[2][kStringCount][kFingerCount] = {};
  position_t positions[2][kStringCount] = {};
  auto map = new map_t [count][kStringCount][kFingerCount];

  uint8_t pitch = pitches[0];

  // first map elements are -1, no previous information
  for (int s = 0; s < kStringCount; ++s) {
    for (int f = 0; f < kStringCount; ++f) {
      map[0][s][f] = -1;
    }
  }

  // set raw scores for first notes
  for (int string = 0; string < kStringCount; ++string) {
    position_t pos = pitch - openStringPitch(string);
    if (pos < 0) {
      positions[0][string] = -1;
      for (int f = 0; f < kFingerCount; ++f) {
        scores[0][string][f] = kPenaltyNever;
      }
      continue;
    }
    positions[0][string] = pos;
    for (int finger = 0; finger < 5; ++finger) {
      scores[0][string][finger] = rawPositionScore(pos, string, finger);
    }
    if (pos) {
      scores[0][string][0] = kPenaltyNever;
    }
  }


  for (int i = 1; i < count; ++i) {
    int current = i & 1;
    int prev = current ^ 1;

    pitch = pitches[i];

    for (int s = 0; s < kStringCount; ++s) {
      position_t pos = pitch - openStringPitch(s);
      if (pos < 0) {
        positions[current][s] = -1;
        for (int f = 0; f < kFingerCount; ++f) {
          scores[current][s][f] = kPenaltyNever;
          map[i][s][f] = -1;
        }
        continue;
      }
      positions[current][s] = pos;

      for (int f = 0; f < kFingerCount; ++f) {
        score_t rawScore = rawPositionScore(pos, s, f);
        string_t best_string = -1;
        finger_t best_finger = -1;
        score_t best_score = kPenaltyMax;
        for (int sPrev = 0; sPrev < kStringCount; ++sPrev) {
          position_t posPrev = positions[prev][sPrev];
          if (posPrev < 0) {
            continue;
          }
          score_t stringScore = stringCrossPenalty(s - sPrev) + rawScore;
          if (stringScore > best_score) {
            continue;
          }
          for (int fPrev = 0; fPrev < kFingerCount; ++fPrev) {
            // compare position difference to finger difference

            score_t fingerScore = stringScore
              + fingerChangePenalty(posPrev, pos, fPrev, f)
              + scores[prev][sPrev][fPrev];
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
        map_t prev_pointer = (best_finger << kFingerBitShift) | best_string;
        scores[current][s][f] = best_score;
        map[i][s][f] = prev_pointer;
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
      if (scores[last][s][f] < best_score) {
        best_score = scores[last][s][f];
        best_string = s;
        best_finger = f;
      }
    }
  }
  FingeringSequence result = FingeringSequence(count, nullptr, nullptr);
  for (int i = count - 1; i > 0; --i) {
    result.setFinger(i, best_finger);
    result.setString(i, best_string);
    map_t m = map[i][best_string][best_finger];
    best_string = m & kStringMask;
    best_finger = m >> kFingerBitShift;
  }
  result.setFinger(0, best_finger);
  result.setString(0, best_string);
  return result;
}
