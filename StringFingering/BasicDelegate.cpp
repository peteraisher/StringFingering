//  Copyright (c) 2022 Peter Aisher
//
//  BasicDelegate.cpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#include "../StringFingering/BasicDelegate.hpp"

namespace string_fingering {

uint8_t BasicDelegate::openStringPitch(size_t forString) {
  static constexpr uint8_t open_pitches[4] = {76, 69, 62, 55};
  return open_pitches[forString];
}

score_t BasicDelegate::rawPositionScore(position_t pos, string_t string,
                                        finger_t finger) {
  if (pos == 0) {
    if (finger) { return kPenaltyNever; }
    return openStringPenalty();
  }
  if (finger == 0) { return kPenaltyNever; }
  // pos > 0, finger > 0
  position_t maxHighFingerPosition = pos +
    maxDistanceToUpperFinger(finger);
  position_t minHighFingerPosition = pos +
    minDistanceToUpperFinger(finger);
  position_t maxLowFingerPosition = pos +
    maxDistanceToLowerFinger(finger);
  position_t minLowFingerPosition = pos +
    minDistanceToLowerFinger(finger);

  // penalise playing in a position below zero
  // i.e. high finger low on fingerboard
  if (maxHighFingerPosition < 6 || maxLowFingerPosition <= 0) {
    return positionTooLowPenalty();
  }

  // penalise playing in high positions
  if (minHighFingerPosition > upperBoutCutoff()) {
    if (minLowFingerPosition >= upperBoutCutoff()) {
      // whole hand over upper bout
      return handBeyondBoutPenalty()
      + minLowFingerPosition - upperBoutCutoff();
    } else {
      // hand partially over upper bout
      return handStraddlingBoutPenalty()
      + minHighFingerPosition - upperBoutCutoff();
    }
  }

  return 0;
}

score_t BasicDelegate::stringCrossPenalty(string_t diff) {
  if (diff < 0)
    diff = -diff;
  if (diff) {
    return singleStringCrossPenalty() * diff;
  } else {
    return 0;
  }
}

score_t
BasicDelegate::fingerChangePenalty(position_t startPos, position_t endPos,
                                   finger_t startFinger, finger_t endFinger,
                                   string_t stringDiff) {
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

void BasicDelegate::reset() {}
void BasicDelegate::nextNote() {}

int BasicDelegate::stringCount() {return 4;}

position_t BasicDelegate::minDistanceToUpperFinger(finger_t finger) const {
  static const position_t distance[5] = {6, 5, 3, 1, 0};
  return distance[finger];
}
position_t BasicDelegate::maxDistanceToUpperFinger(finger_t finger) const {
  static const position_t distance[5] = {kPositionCount, 5, 4, 2, 0};
  return distance[finger];
}
position_t BasicDelegate::minDistanceToLowerFinger(finger_t finger) const {
  static const position_t distance[5] = {1, 0, -2, -4, -5};
  return distance[finger];
}
position_t BasicDelegate::maxDistanceToLowerFinger(finger_t finger) const {
  static const position_t distance[5] = {kPositionCount, 0, -1, -3, -5};
  return distance[finger];
}
position_t BasicDelegate::upperBoutCutoff() const {
  return 8;
}

position_t BasicDelegate::maxComfortableFingerChange(finger_t start,
                                                     finger_t end) const {
  if (!start) return kPositionCount;
  if (!end)   return 0;
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

position_t BasicDelegate::minComfortableFingerChange(finger_t start,
                                                     finger_t end) const {
  return -maxComfortableFingerChange(end, start);
}

score_t BasicDelegate::handStraddlingBoutPenalty() const {
  return (3 * kPenaltyMedium) / 2;
}
score_t BasicDelegate::handBeyondBoutPenalty() const {
  return 2 * kPenaltyMedium;
}
score_t BasicDelegate::openStringPenalty() const {
  return kPenaltyMedium;
}
score_t BasicDelegate::positionTooLowPenalty() const {
  return kPenaltyMedium;
}
score_t BasicDelegate::singleStringCrossPenalty() const {
  return kPenaltyLow * 2;
}
score_t BasicDelegate::shiftPenalty(position_t shiftAmount) const {
  if (shiftAmount < 0)
    shiftAmount = -shiftAmount;
  return basicShiftPenalty() + perSemitoneShiftPenalty() * shiftAmount;
}
score_t BasicDelegate::basicShiftPenalty() const {return kPenaltyMedium;}
score_t BasicDelegate::perSemitoneShiftPenalty() const {return kPenaltyLow;}

}   // namespace string_fingering
