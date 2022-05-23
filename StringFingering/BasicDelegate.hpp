//  Copyright (c) 2022 Peter Aisher
//
//  BasicDelegate.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef STRINGFINGERING_BASICDELEGATE_HPP_
#define STRINGFINGERING_BASICDELEGATE_HPP_

#include <limits>
#include "../StringFingering/Optimizer.hpp"

namespace string_fingering {

class BasicDelegate : public OptimizerDelegate {
  enum {
    kPenaltyLow = 1,
    kPenaltyMedium = 50,
    kPenaltyHigh = 1000,
    kPenaltyNever = 100000,
    kPenaltyMax = std::numeric_limits<score_t>::max()
  };
  enum {
    kPositionCount = 32
  };

 public:
  uint8_t openStringPitch(size_t forString) override;
  /// \note value is not assumed to be stable between calls
  score_t rawPositionScore(position_t pos, string_t onString,
                                   finger_t finger) override;
  /// \note value is not assumed to be stable between calls
  score_t stringCrossPenalty(string_t diff) override;
  /// \note value is not assumed to be stable between calls
  score_t fingerChangePenalty(position_t startPos, position_t endPos,
                                      finger_t startFinger, finger_t endFinger,
                                      string_t stringDiff) override;
  score_t never() const override {return kPenaltyNever;}
  void nextNote() override;
  void reset() override;
  int stringCount() override;

 private:
  position_t minDistanceToUpperFinger(finger_t finger) const;
  position_t maxDistanceToUpperFinger(finger_t finger) const;
  position_t minDistanceToLowerFinger(finger_t finger) const;
  position_t maxDistanceToLowerFinger(finger_t finger) const;
  position_t upperBoutCutoff() const;

  position_t maxComfortableFingerChange(finger_t start, finger_t end) const;
  position_t minComfortableFingerChange(finger_t start, finger_t end) const;

  score_t handStraddlingBoutPenalty() const;
  score_t handBeyondBoutPenalty() const;
  score_t openStringPenalty() const;
  score_t positionTooLowPenalty() const;
  score_t singleStringCrossPenalty() const;
  score_t shiftPenalty(position_t shiftAmount) const;
  score_t basicShiftPenalty() const;
  score_t perSemitoneShiftPenalty() const;
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_BASICDELEGATE_HPP_
