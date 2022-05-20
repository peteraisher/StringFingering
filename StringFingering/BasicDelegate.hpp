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

private:

  position_t minDistanceToUpperFinger(finger_t finger) const;
  position_t maxDistanceToUpperFinger(finger_t finger) const;
  position_t minDistanceToLowerFinger(finger_t finger) const;
  position_t maxDistanceToLowerFinger(finger_t finger) const;
  position_t upperBoutCutoff() const;

  score_t handStraddlingBoutPenalty() const;
  score_t handBeyondBoutPenalty() const;
  score_t openStringPenalty() const;
  score_t positionTooLowPenalty() const;
  score_t singleStringCrossPenalty() const;

  score_t shiftPenalty(position_t shiftAmount) {
    if (shiftAmount < 0)
      shiftAmount = -shiftAmount;
    return basicShiftPenalty() + perSemitoneShiftPenalty() * shiftAmount;
  }

  score_t basicShiftPenalty() {return kPenaltyMedium;}
  score_t perSemitoneShiftPenalty() {return kPenaltyLow;}

  position_t maxComfortableFingerChange(finger_t start, finger_t end) {
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

  position_t minComfortableFingerChange(finger_t start, finger_t end) {
    return -maxComfortableFingerChange(end, start);
  }
};

}   // namespace string_fingering

#endif /* BasicDelegate_hpp */
