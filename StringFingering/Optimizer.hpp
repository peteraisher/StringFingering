//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.hpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#ifndef STRINGFINGERING_OPTIMIZER_HPP_
#define STRINGFINGERING_OPTIMIZER_HPP_

#include <cstdint>
#include <limits>
#include "../StringFingering/OptimizerDelegate.hpp"
#include "../StringFingering/SingleNoteSequence.hpp"
#include "../StringFingering/FingeringSequence.hpp"
#include "../StringFingering/types.h"


namespace string_fingering {

class Optimizer {
 public:
  explicit Optimizer(OptimizerDelegate* delegate);
  FingeringSequence calculate(const SingleNoteSequence& sequence);
 private:
  OptimizerDelegate* delegate;
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_OPTIMIZER_HPP_
