//
//  StringFingeringOptimizer.hpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#ifndef StringFingeringOptimizer_hpp
#define StringFingeringOptimizer_hpp

#include "SingleNoteSequence.hpp"
#include "FingeringSequence.hpp"

#include <cstdint>
#include <limits>



class StringFingeringOptimizerDelegate {
public:
  typedef int32_t score_t;
  typedef int8_t position_t;
  typedef int8_t map_t;
  typedef int8_t finger_t;
  typedef int8_t string_t;

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
  virtual void nextNote() = 0;
  virtual void reset() = 0;
};

template<int kNumStrings = 4, int kPositionCount = 32>
class StringFingeringOptimizer {
public:
  using score_t = StringFingeringOptimizerDelegate::score_t;
  using position_t = StringFingeringOptimizerDelegate::position_t;
  using map_t = StringFingeringOptimizerDelegate::map_t;
  using finger_t = StringFingeringOptimizerDelegate::finger_t;
  using string_t = StringFingeringOptimizerDelegate::string_t;
  StringFingeringOptimizer(StringFingeringOptimizerDelegate* delegate)
  : delegate(delegate), kStringBits(0), kStringMask(1) {
    while (kNumStrings > kStringMask) {
      kStringMask <<= 1;
      ++kStringBits;
    }
    --kStringMask;
    populateOpenStringPitches();
  }

  FingeringSequence calculate(const SingleNoteSequence& sequence);

private:

  void populateOpenStringPitches() {
    for (int i = 0; i < kNumStrings; ++i) {
      openStringPitches[i] = delegate->openStringPitch(i);
    }
  }

  int kStringBits;
  int kStringMask;
  uint8_t openStringPitches[kNumStrings];
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
  StringFingeringOptimizerDelegate* delegate;
};

#endif /* StringFingeringOptimizer_hpp */
