//  Copyright (c) 2022 Peter Aisher
//
//  BasicDelegate.hpp
//  StringFingering
//
//  Created by Peter Aisher on 20.05.2022.
//

#ifndef BasicDelegate_hpp
#define BasicDelegate_hpp

#include "Optimizer.hpp"

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
  virtual uint8_t openStringPitch(int string) override;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t rawPositionScore(position_t pos, string_t string,
                                   finger_t finger) override;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t stringCrossPenalty(string_t diff) override;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t fingerChangePenalty(position_t startPos, position_t endPos,
                                      finger_t startFinger, finger_t endFinger,
                                      string_t stringDiff) override;
  virtual void nextNote() override;
  virtual void reset() override;
  virtual int stringCount() override;

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

#endif /* BasicDelegate_hpp */
