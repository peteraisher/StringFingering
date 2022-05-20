//  Copyright (c) 2022 Peter Aisher
//
//  main.cpp
//  StringFingering
//
//  Created by Peter Aisher on 28.09.21.
//

#include "Optimizer.hpp"
#include "BasicDelegate.hpp"
#include "SingleNoteSequence.hpp"
#include "FingeringSequence.hpp"
#include <stdio.h>
#include <array>
#include <string>

void printNoteNameAndOctave(uint8_t pitch) {
  std::string names[12] = {"C", "C#", "D", "D#", "E", "F",
                           "F#", "G", "G#", "A", "A#", "B"};
  std::string name = names[pitch % 12];
  int octave = pitch / 12;
  octave -= 1;

  printf("%s%i", name.c_str(), octave);
}

int main(int argc, const char * argv[]) {
  auto delegate = string_fingering::BasicDelegate();

  auto sf = string_fingering::Optimizer(&delegate);

  std::vector<uint8_t> in_pitches {
    60, 62, 64, 65, 67, 69, 71, 72,
    74, 72, 71, 69, 67, 65, 64, 62,
    60, 64, 67, 72, 76, 72, 76, 79,
    84, 79, 76, 72, 67, 72, 67, 64,
    60
  };
  auto notes = string_fingering::SingleNoteSequence(in_pitches);

  auto fingering = sf.calculate(notes);

  size_t count = fingering.getCount();
  auto pitches = notes.getPitches();
  printf("note  f  s\n");
  for (int i = 0; i < count; ++i) {
    printf(" ");
    printNoteNameAndOctave(pitches[i]);
    printf("  %2d %2d\n", fingering.getFinger(i), fingering.getString(i));
  }

  return 0;
}
