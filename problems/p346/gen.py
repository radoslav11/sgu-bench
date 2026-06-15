"""Tests for p346 (Snooker): 7 ball counts + state word.

The whole input space is tiny: reds 0..15 (with all 6 colours present
when reds >= 1) or reds = 0 with the remaining colours forming a suffix
of yellow..black; state is RED/COLOUR/NONE. Seed 1 is the empty table,
seeds 2-3 the maximal table in RED/COLOUR states, seeds 4-7 the
statement samples, the rest random valid combinations (including every
suffix length for reds = 0).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(reds, colours, state):
    print(" ".join(str(x) for x in [reds] + colours))
    print(state)


if seed == 1:
    emit(0, [0, 0, 0, 0, 0, 0], "NONE")
elif seed == 2:
    emit(15, [1, 1, 1, 1, 1, 1], "RED")
elif seed == 3:
    emit(15, [1, 1, 1, 1, 1, 1], "COLOUR")
elif seed == 4:
    emit(2, [1, 1, 1, 1, 1, 1], "COLOUR")
elif seed == 5:
    emit(2, [1, 1, 1, 1, 1, 1], "RED")
elif seed == 6:
    emit(0, [0, 0, 1, 1, 1, 1], "NONE")
elif seed == 7:
    emit(0, [1, 1, 1, 1, 1, 1], "RED")
elif seed == 8:
    # Only the black ball remains.
    emit(0, [0, 0, 0, 0, 0, 1], "COLOUR")
else:
    if rng.random() < 0.5:
        reds = rng.randint(1, 15)
        colours = [1, 1, 1, 1, 1, 1]
        state = rng.choice(["RED", "COLOUR", "NONE"])
    else:
        reds = 0
        k = rng.randint(0, 6)  # number of colours already gone
        colours = [0] * k + [1] * (6 - k)
        if k == 0:
            state = rng.choice(["RED", "COLOUR", "NONE"])
        else:
            state = rng.choice(["COLOUR", "NONE"])
    emit(reds, colours, state)
