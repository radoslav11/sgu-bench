"""Tests for p397 (Text Editor): one string of 'a'-'z', 'L', 'R',
length 1..10^6.

Seed 1 is a single letter, seed 2 a maximal random mix, seed 3 a maximal
front-insertion pattern ("aL" repeated) that forces O(n^2) work out of
array-based editors. Later seeds cover pure letters, cursor bouncing
against both ends, and random letter/command ratios.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXLEN = 10**6
LETTERS = string.ascii_lowercase

if seed == 1:
    print("a")
elif seed == 2:
    print("".join(rng.choice(LETTERS + "LR") for _ in range(MAXLEN)))
elif seed == 3:
    # every insertion happens at the front of the text
    print("".join(rng.choice(LETTERS) + "L" for _ in range(MAXLEN // 2)))
elif seed == 4:
    print("".join(rng.choice(LETTERS) for _ in range(MAXLEN)))
elif seed == 5:
    # type a block, then walk the cursor far past both ends, repeat
    parts = []
    total = 0
    while total < MAXLEN:
        block = rng.randint(1, 2000)
        chunk = "".join(rng.choice(LETTERS) for _ in range(block))
        walk = "L" * rng.randint(0, 2 * block) + "R" * rng.randint(0, block)
        piece = (chunk + walk)[: MAXLEN - total]
        parts.append(piece)
        total += len(piece)
    print("".join(parts))
elif seed == 6:
    print("L" * (MAXLEN // 2) + "R" * (MAXLEN // 2))
else:
    n = rng.randint(2, MAXLEN)
    p_letter = rng.uniform(0.2, 0.9)
    out = []
    for _ in range(n):
        if rng.random() < p_letter:
            out.append(rng.choice(LETTERS))
        else:
            out.append(rng.choice("LR"))
    print("".join(out))
