"""Tests for p436 (The Diputs notation): input and output <= 1 MB.

The Diputs system is mixed-radix with digit capacities 2,3,5,7,11,13,
17,19,23 (least significant first), max value 836075519. Seed 1 is the
single zero "O". Seed 2 is ~900 KB of long Diputs numbers (odd count,
so the full list gets sorted); seed 3 is decimal-heavy sized so that
the expanded Diputs output stays under 1 MB. Seed 4 packs greedy-parse
traps (runs like "___", "020", digit overruns, capacity overruns) and
the rest are random mixes of numbers and plain text.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

DIGITS = "_.,-~='^\""
CAPS = [2, 3, 5, 7, 11, 13, 17, 19, 23]
PROD = [1]
for c in CAPS:
    PROD.append(PROD[-1] * (c + 1))
MAX_NUM = PROD[9] - 1  # 836075519


def to_diputs(v):
    if v == 0:
        return "O"
    res = []
    for i in range(8, -1, -1):
        res.append(DIGITS[i] * ((v // PROD[i]) % (CAPS[i] + 1)))
    return "".join(res)


SAFE = "abcdefghijklmnpqrstuvwxyzABCDEFGHIJKLMNPQRSTUVWXYZ#$%&*"


def sep():
    return rng.choice(["\n", " ", rng.choice(SAFE), rng.choice(SAFE)])


if seed == 1:
    sys.stdout.write("O\n")
elif seed == 2:
    # Maximal test: long Diputs numbers (decimal output is short), odd
    # token count so all of them must be sorted.
    parts = []
    size = 0
    count = 0
    while size < 900000:
        v = rng.randint(PROD[7], MAX_NUM)  # long representations
        tok = to_diputs(v)
        parts.append(tok)
        parts.append(sep())
        size += len(tok) + 1
        count += 1
    if count % 2 == 0:
        parts.append("O\n")
    sys.stdout.write("".join(parts))
elif seed == 3:
    # Maximal decimal-heavy test: output expands ~7x, so cap by the
    # exact converted size instead of the input size.
    parts = []
    in_size = 0
    out_size = 0
    count = 0
    while in_size < 900000 and out_size < 900000:
        v = rng.randint(0, MAX_NUM)
        tok = str(v)
        parts.append(tok)
        parts.append(sep())
        in_size += len(tok) + 1
        out_size += len(to_diputs(v)) + 1
        count += 1
    if count % 2 == 0:
        parts.append("O\n")
    sys.stdout.write("".join(parts))
elif seed == 4:
    # Greedy-parsing traps, kept small.
    sys.stdout.write(
        "___\n"
        "020\n"
        "_12\n"
        "OO00O\n"
        + '"' * 30 + "\n"
        + "_" * 7 + "\n"
        + ".,_\n"            # digits in wrong order split into pieces
        + "836075519836075520\n"  # overflow forces a greedy split
        + "9" * 60 + "\n"
        + "'='~\n"
        + to_diputs(MAX_NUM) + to_diputs(MAX_NUM) + "\n"
    )
elif seed == 5:
    # No numbers at all: text passes through unchanged.
    sys.stdout.write("hello brave new world without any tokens\n")
elif seed == 6:
    # One huge digit string: split into many maximal decimals (the
    # count parity here exercises the sorting rule too).
    sys.stdout.write("1234567890" * 50 + "\n")
else:
    # Random mixes of decimals, Diputs numbers, zeros and junk text.
    parts = []
    for _ in range(rng.randint(1, 4000)):
        roll = rng.random()
        if roll < 0.35:
            parts.append(str(rng.randint(0, MAX_NUM)))
        elif roll < 0.7:
            parts.append(to_diputs(rng.randint(0, MAX_NUM)))
        elif roll < 0.8:
            parts.append("O")
        else:
            parts.append(
                "".join(rng.choice(SAFE) for _ in range(rng.randint(1, 8)))
            )
        parts.append(sep())
    sys.stdout.write("".join(parts).rstrip() + "\n")
