//
//  main.cpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#include "StringFingering.hpp"
#include "SingleNoteSequence.hpp"
#include "FingeringSequence.hpp"
#include <stdio.h>

int main(int argc, const char * argv[]) {
  StringFingering sf = StringFingering();

  uint8_t in_pitches[33] = {
    60, 62, 64, 65, 67, 69, 71, 72,
    74, 72, 71, 69, 67, 65, 64, 62,
    60, 64, 67, 72, 76, 72, 76, 79,
    84, 79, 76, 72, 67, 72, 67, 64,
    60
  };
  SingleNoteSequence notes = SingleNoteSequence(33, in_pitches);

  FingeringSequence fingering = sf.optimalFingeringFor(notes);

  int count = fingering.getCount();
  uint8_t* pitches = notes.getPitches();
  printf("note  f  s\n");
  for (int i = 0; i < count; ++i) {
    printf("%3d  %2d %2d\n", pitches[i], fingering.getFinger(i), fingering.getString(i));
  }

  return 0;
}