# StringFingering
A generator of string instrument fingerings based on transition penalties

# Quick Start
All classes are in the namespace `string_fingering`.

The class which calculates the string fingering is `Optimizer`.

Optimizer takes an instance of a subclass of `OptimizerDelegate`, which may be a custom subclass.

A class `BasicDelegate` is included as an example.
