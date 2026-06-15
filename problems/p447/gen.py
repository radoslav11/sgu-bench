"""Tests for p447 (Optimal Rest): re-encode MML rest commands optimally.

Input is one line of valid rest commands, at most 100000 characters. A
command is R + base (1,2,4,8,16,32,64) + dots, the dotted chain never
dropping below 1/64. Seed 2 is 50000 x "R1" (maximal total duration,
heaviest DP), seed 3 a 100K-char random mix of all 28 commands.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_DOTS = {1: 6, 2: 5, 4: 4, 8: 3, 16: 2, 32: 1, 64: 0}
ALL_CMDS = [
    "R" + str(b) + "." * d
    for b in (1, 2, 4, 8, 16, 32, 64)
    for d in range(MAX_DOTS[b] + 1)
]


def fill(picker, limit=100000):
    parts = []
    used = 0
    while True:
        c = picker()
        if used + len(c) > limit:
            break
        parts.append(c)
        used += len(c)
    return "".join(parts)


if seed == 1:
    print("R64")
elif seed == 2:
    # Maximal duration: 50000 whole rests, 100000 chars, V = 3,200,000.
    print("R1" * 50000)
elif seed == 3:
    # Maximal length, random mix of all 28 commands.
    print(fill(lambda: rng.choice(ALL_CMDS)))
elif seed == 4:
    # Maximal count of tiny rests: only R64/R32 commands.
    print(fill(lambda: rng.choice(["R64", "R32", "R32."])))
elif seed == 5:
    # Fully dotted whole rests: value 127 each, lots of 1/64 leftovers.
    print("R1......" * 12500)
elif seed == 6:
    # Single longest command.
    print("R1......")
elif seed == 7:
    # Odd total in 1/64 units: forces a trailing R64 in the optimum.
    print("R1" * 1000 + "R64")
else:
    # Random small/medium sequences.
    limit = rng.randint(2, 3000)
    s = fill(lambda: rng.choice(ALL_CMDS), limit)
    if not s:
        s = "R4."
    print(s)
