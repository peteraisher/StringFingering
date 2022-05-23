//  Copyright (c) 2022 Peter Aisher
//
//  SingleNoteSequence.cpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#include <algorithm>
#include "../StringFingering/SingleNoteSequence.hpp"

namespace string_fingering {

SingleNoteSequence::SingleNoteSequence(std::vector<uint8_t> pitches_in) {
  std::copy(pitches_in.begin(), pitches_in.end(),
            std::back_inserter(pitches));
}

}   // namespace string_fingering
