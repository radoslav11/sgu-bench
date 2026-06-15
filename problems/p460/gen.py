"""Tests for p460 (Plural Form of Nouns): n <= 10 words of 2..25 letters.

Seed 1 is minimal (one 2-letter word), seeds 2-3 are maximal (10 words
of 25 letters) with endings that exercise every rule: y, ch, x, s, o,
f, fe, ff, ffe, h without c, and plain endings. Later seeds are random
words biased toward the special suffixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

SUFFIXES = [
    "y", "ch", "x", "s", "o", "f", "fe", "ff", "ffe", "h", "sh",
    "th", "e", "cy", "xy", "fy", "oy", "sy", "chy", "yf", "ye", "",
]


def word(length, suffix):
    body = "".join(
        rng.choice("abcdefghijklmnopqrstuvwxyz")
        for _ in range(length - len(suffix))
    )
    return body + suffix


if seed == 1:
    print(1)
    print(word(2, "y"))
elif seed == 2:
    print(10)
    for sfx in ["y", "ch", "x", "s", "o", "f", "fe", "ff", "ffe", "h"]:
        print(word(25, sfx))
elif seed == 3:
    print(10)
    for sfx in ["sh", "th", "chy", "yf", "ye", "cy", "xy", "fy", "oy", ""]:
        print(word(25, sfx))
else:
    n = rng.randint(1, 10)
    print(n)
    for _ in range(n):
        sfx = rng.choice(SUFFIXES)
        print(word(rng.randint(max(2, len(sfx)), 25), sfx))
