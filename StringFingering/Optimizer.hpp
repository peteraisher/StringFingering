//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.hpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#ifndef StringFingeringOptimizer_hpp
#define StringFingeringOptimizer_hpp

#include "SingleNoteSequence.hpp"
#include "FingeringSequence.hpp"
#include "types.h"

#include <cstdint>
#include <limits>

namespace string_fingering {

class OptimizerDelegate {
public:

  /// \note called once per string upon initialization and cached
  virtual uint8_t openStringPitch(int string) = 0;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t rawPositionScore(position_t pos, string_t string,
                                   finger_t finger) = 0;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t stringCrossPenalty(string_t diff) = 0;
  /// \note value is not assumed to be stable between calls with same parameter values
  virtual score_t fingerChangePenalty(position_t startPos, position_t endPos,
                                      finger_t startFinger, finger_t endFinger,
                                      string_t stringDiff) = 0;
  virtual int stringCount() = 0;
  virtual void nextNote() = 0;
  virtual void reset() = 0;
};

template<int kNumStrings = 4>
class Optimizer {
public:
  Optimizer(OptimizerDelegate* delegate);
  FingeringSequence calculate(const SingleNoteSequence& sequence);

private:

  int kStringBits;
  int kStringMask;
  enum {
    kFingerCount = 5,
  };
  enum {
    kPenaltyLow = 1,
    kPenaltyMedium = 50,
    kPenaltyHigh = 1000,
    kPenaltyNever = 100000,
    kPenaltyMax = std::numeric_limits<score_t>::max()
  };
  OptimizerDelegate* delegate;
};

}   // namespace string_fingering

#endif /* StringFingeringOptimizer_hpp */
