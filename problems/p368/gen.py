"""Tests for p368 (Tests): grade conversion and specialty acceptance.

N, M <= 100; scores 0..100; per faculty K subjects with X < Y < Z
thresholds, S <= 100 specialties. Names are 1..100 chars of Latin
letters and single spaces (no leading/trailing space). Seed 1 is the
minimal case, seeds 2-3 are maximal (100 faculties x 100 specialties
with 100-char names), seed 4 forces the "Army" answer.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = string.ascii_letters


def name(max_len):
    length = rng.randint(1, max_len)
    chars = [rng.choice(LETTERS)]
    while len(chars) < length:
        if chars[-1] != " " and len(chars) < length - 1 and rng.random() < 0.15:
            chars.append(" ")
        else:
            chars.append(rng.choice(LETTERS))
    return "".join(chars)


def triplet():
    x, y, z = sorted(rng.sample(range(1, 101), 3))
    return f"{x} {y} {z}"


if seed == 1:
    print(1, 1)
    print(0)
    print("A")
    print(1, 1)
    print("1 2 3")
    print(1)
    print("B")
    print(2)
    sys.exit()

if seed in (2, 3):
    n, m, max_k, max_s, name_len = 100, 100, 100, 100, 100
elif seed == 4:
    # Everything rejected: requirements far above any achievable sum.
    n, m, max_k, max_s, name_len = 50, 100, 50, 20, 60
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    max_k = m
    max_s = 100
    name_len = 100

scores = [rng.randint(0, 100) for _ in range(m)]
out = [f"{n} {m}", " ".join(map(str, scores))]

for _ in range(n):
    out.append(name(name_len))
    k = max_k if seed in (2, 3) else rng.randint(1, max_k)
    subjects = [rng.randint(1, m) for _ in range(k)]
    out.append(f"{k} " + " ".join(map(str, subjects)))
    out.append(" ".join(triplet() for _ in range(k)))
    s = max_s if seed in (2, 3) else rng.randint(1, max_s)
    out.append(str(s))
    for _ in range(s):
        out.append(name(name_len))
        if seed == 4:
            required = rng.randint(5 * k + 1, 5 * k + 100)
        elif seed == 3:
            # Many exact-boundary requirements around the typical sum.
            required = rng.randint(3 * k, 4 * k)
        else:
            required = rng.randint(0, 5 * k + 5)
        out.append(str(required))

print("\n".join(out))
