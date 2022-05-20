//  Copyright (c) 2022 Peter Aisher
//
//  Optimizer.cpp
//  StringFingering
//
//  Created by Peter Aisher on 29.09.21.
//

#include "Optimizer.hpp"
#include "PathFinder.hpp"
#include <vector>

namespace string_fingering {

Optimizer::Optimizer(OptimizerDelegate* delegate) : delegate(delegate) {}

FingeringSequence Optimizer::calculate(const SingleNoteSequence& sequence) {
  auto count = sequence.getCount();
  auto pathFinder = PathFinder(count, delegate);
  pathFinder.setFirstPitch(sequence[0]);
  for (int i = 1; i < count; ++i) {
    pathFinder.appendPitch(sequence[i]);
  }
  return pathFinder.readOutput();
}

}   // namespace string_fingering
