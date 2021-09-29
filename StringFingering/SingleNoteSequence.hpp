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

class SingleNoteSequence {
 public:
  SingleNoteSequence(int count, uint8_t* pitches_in) : count(count) {
    pitches = new uint8_t[count];
    if (pitches_in)
      memcpy(pitches, pitches_in, count);
  }
  ~SingleNoteSequence() {}
  inline int getCount() {return count;}
  inline uint8_t* getPitches() {return pitches;}
 private:
  int count;
  uint8_t* pitches;
};

#endif /* SingleNoteSequence_hpp */
