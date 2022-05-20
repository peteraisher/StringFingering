//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.hpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#ifndef StringFingeringOptimizer_hpp
#define StringFingeringOptimizer_hpp

#include "OptimizerDelegate.hpp"
#include "SingleNoteSequence.hpp"
#include "FingeringSequence.hpp"
#include "types.h"

#include <cstdint>
#include <limits>

namespace string_fingering {

class Optimizer {
public:
  Optimizer(OptimizerDelegate* delegate);
  FingeringSequence calculate(const SingleNoteSequence& sequence);
private:
  OptimizerDelegate* delegate;
};

}   // namespace string_fingering

#endif /* StringFingeringOptimizer_hpp */
