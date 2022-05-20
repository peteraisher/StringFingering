//
//  SingleNoteSequence.hpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#ifndef SingleNoteSequence_hpp
#define SingleNoteSequence_hpp
#include <cstdint>
#include <cstring>
#include <vector>

namespace string_fingering {

class SingleNoteSequence {
  std::vector<uint8_t> pitches {};
 public:

  SingleNoteSequence(std::vector<uint8_t> pitches_in) {
    std::copy(pitches_in.begin(), pitches_in.end(),
              std::back_inserter(pitches));
  }
  inline size_t getCount() const {return pitches.size();}
  inline const std::vector<uint8_t>& getPitches() const {return pitches;}
  inline const uint8_t& operator[] (size_t i) const {return pitches[i];}
};

}   // namespace string_fingering

#endif /* SingleNoteSequence_hpp */
