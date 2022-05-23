//  Copyright (c) 2022 Peter Aisher
//
//  SingleNoteSequence.hpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#ifndef STRINGFINGERING_SINGLENOTESEQUENCE_HPP_
#define STRINGFINGERING_SINGLENOTESEQUENCE_HPP_
#include <cstdint>
#include <cstring>
#include <vector>

namespace string_fingering {

class SingleNoteSequence {
  std::vector<uint8_t> pitches {};
 public:
  explicit SingleNoteSequence(std::vector<uint8_t> pitches_in);
  inline size_t getCount() const {return pitches.size();}
  inline const std::vector<uint8_t>& getPitches() const {return pitches;}
  inline const uint8_t& operator[] (size_t i) const {return pitches[i];}
};

}   // namespace string_fingering

#endif  // STRINGFINGERING_SINGLENOTESEQUENCE_HPP_
