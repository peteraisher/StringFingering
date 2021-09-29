//
//  StringFingering.hpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#ifndef StringFingering_hpp
#define StringFingering_hpp

#include "FingeringSequence.hpp"
#include "SingleNoteSequence.hpp"
#include <cstdint>
#include <cassert>
#include <limits>



class StringFingering {
public:
  StringFingering() {
    initRawPositionScores();
  }

  FingeringSequence optimalFingeringFor(SingleNoteSequence noteSequence);

private:

  typedef int32_t score_t;
  typedef int8_t position_t;
  typedef int8_t map_t;
  typedef int8_t finger_t;
  typedef int8_t string_t;

  score_t mRawPositionScores[32][4][5];

  void initRawPositionScores() {
    // open string must be played with finger 0
    for (int string = 0; string < kStringCount; ++string) {
      mRawPositionScores[0][string][0] = openStringPenalty();
      // cannot be played with any other finger
      for (int finger = 1; finger < kFingerCount; ++finger)
        mRawPositionScores[0][string][finger] = kPenaltyNever;
    }
    for (int position = 1; position < kPositionCount; ++position) {
      for (int string = 0; string < kStringCount; ++string) {
        // non-open positions cannot be played with finger 0
        mRawPositionScores[position][string][0] = kPenaltyNever;
        for (finger_t finger = 1; finger < kFingerCount; ++finger) {
          position_t maxHighFingerPosition = position +
            maxDistanceToUpperFinger(finger);
          position_t minHighFingerPosition = position +
            minDistanceToUpperFinger(finger);
          position_t maxLowFingerPosition = position +
            maxDistanceToLowerFinger(finger);
          position_t minLowFingerPosition = position +
            minDistanceToLowerFinger(finger);

          // penalise playing in a position below zero
          // i.e. high finger low on fingerboard
          if (maxHighFingerPosition < 6 || maxLowFingerPosition <= 0) {
            mRawPositionScores[position][string][finger] =
              positionTooLowPenalty();
            continue;
          }

          // penalise playing in high positions
          if (minHighFingerPosition > upperBoutCutoff()) {
            if (minLowFingerPosition >= upperBoutCutoff()) {
              // whole hand over upper bout
              mRawPositionScores[position][string][finger]
              = handBeyondBoutPenalty()
              + minLowFingerPosition - upperBoutCutoff();
            } else {
              // hand partially over upper bout
              mRawPositionScores[position][string][finger]
              = handStraddlingBoutPenalty()
              + minHighFingerPosition - upperBoutCutoff();
            }
            continue;
          }

          mRawPositionScores[position][string][finger] = 0;
        }
      }
    }
  }

  // TODO: these functions should be different for contrabass
  inline position_t minDistanceToUpperFinger(finger_t finger) const {
    static const position_t distance[5] = {6, 5, 3, 1, 0};
    return distance[finger];
  }

  inline position_t maxDistanceToUpperFinger(finger_t finger) const {
    static const position_t distance[5] = {kPositionCount, 5, 4, 2, 0};
    return distance[finger];
  }

  inline position_t minDistanceToLowerFinger(finger_t finger) const {
    static const position_t distance[5] = {1, 0, -2, -4, -5};
    return distance[finger];
  }
  inline position_t maxDistanceToLowerFinger(finger_t finger) const {
    static const position_t distance[5] = {kPositionCount, 0, -1, -3, -5};
    return distance[finger];
  }


  inline score_t handStraddlingBoutPenalty() const {
    return (3 * kPenaltyMedium) / 2;
  }

  inline score_t handBeyondBoutPenalty() const {
    return 2 * kPenaltyMedium;
  }

  inline score_t openStringPenalty() const {
    return kPenaltyMedium;
  }

  inline score_t positionTooLowPenalty() const {
    return kPenaltyMedium;
  }

  inline score_t fingerChangePenalty(position_t startPos, position_t endPos,
                                     finger_t startFinger, finger_t endFinger) {
    position_t maxComfortableChange = maxComfortableFingerChange(startFinger,
                                                                 endFinger);
    position_t minComfortableChange = minComfortableFingerChange(startFinger,
                                                                 endFinger);
    position_t actualChange = endPos - startPos;

    if (actualChange < minComfortableChange)
      return shiftPenalty(actualChange - minComfortableChange);
    if (actualChange > maxComfortableChange)
      return shiftPenalty(actualChange - maxComfortableChange);
    return 0;
  }

  inline score_t shiftPenalty(position_t shiftAmount) {
    if (shiftAmount < 0)
      shiftAmount = -shiftAmount;
    return basicShiftPenalty() + perSemitoneShiftPenalty() * shiftAmount;
  }

  inline score_t basicShiftPenalty() {return kPenaltyMedium;}
  inline score_t perSemitoneShiftPenalty() {return kPenaltyLow;}

  inline position_t maxComfortableFingerChange(finger_t start, finger_t end) {
    if (!start)
      return kPositionCount;
    if (!end) return 0;
    if (!start || !end) {
      return kPositionCount; // to or from open string
    }
    finger_t diff = end - start;
    switch (diff) {
      case -3:  return -5;
      case -2:  return -3;
      case -1:  return -1;
      case  0:  return  0;
      case  1:  return  2;
      case  2:  return  4;
      case  3:  return  5;
    }
    return -kPositionCount;
  }

  inline position_t minComfortableFingerChange(finger_t start, finger_t end) {
    return -maxComfortableFingerChange(end, start);
  }

  inline score_t stringCrossPenalty(string_t diff) const {
    if (diff < 0)
      diff = -diff;
    if (diff) {
      return singleStringCrossPenalty() * diff;
    } else {
      return 0;
    }
  }

  inline score_t singleStringCrossPenalty() const {
    return kPenaltyLow * 2;
  }

  inline position_t upperBoutCutoff() const {
    return 8;
  }

  inline score_t rawPositionScore(position_t pos, string_t string,
                                  finger_t finger) const {
    return mRawPositionScores[pos][string][finger];
  }

  // TODO: this changes for different instruments
  inline uint8_t openStringPitch(string_t string) {
    static const uint8_t open_pitches[4] = {76, 69, 62, 55};
    return open_pitches[string];
  }
  enum {
    kPenaltyLow = 1,
    kPenaltyMedium = 50,
    kPenaltyHigh = 1000,
    kPenaltyNever = 100000,
    kPenaltyMax = std::numeric_limits<score_t>::max()
  };
  enum {
    kStringCount = 4,
    kFingerCount = 5,
    kPositionCount = 32,
    kStringMask = 3,
    kFingerBitShift = 2,
  };

};

#endif /* StringFingering_hpp */
