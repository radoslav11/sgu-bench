"""Tests for p307 (Cipher): fingerprint of 2x2 sums, 2 <= H, W <= 300.

Most seeds build the fingerprint from a hidden random 0/1 key, so a key
exists. Seed 1 is the minimal 2x2 case, seeds 2-3 are 300x300 (one
solvable, one with uniformly random fingerprint digits, which is almost
surely CORRUPT). Later seeds cover thin strips (H=2 / W=2), biased keys,
and small random fingerprints that mix solvable and corrupt cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def from_key(h, w, p_one):
    key = [[1 if rng.random() < p_one else 0 for _ in range(w)] for _ in range(h)]
    rows = []
    for i in range(h - 1):
        rows.append(
            "".join(
                str(key[i][j] + key[i + 1][j] + key[i][j + 1] + key[i + 1][j + 1])
                for j in range(w - 1)
            )
        )
    return h, w, rows


def random_fp(h, w):
    rows = ["".join(str(rng.randint(0, 4)) for _ in range(w - 1)) for _ in range(h - 1)]
    return h, w, rows


if seed == 1:
    h, w, rows = from_key(2, 2, 0.5)
elif seed == 2:
    h, w, rows = from_key(300, 300, 0.5)
elif seed == 3:
    h, w, rows = random_fp(300, 300)
elif seed == 4:
    h, w, rows = from_key(2, 300, 0.5)
elif seed == 5:
    h, w, rows = from_key(300, 2, 0.3)
elif seed == 6:
    h, w, rows = from_key(299, 300, 0.05)
elif seed == 7:
    h, w, rows = from_key(300, 299, 0.95)
elif seed == 8:
    h, w, rows = random_fp(rng.randint(2, 8), rng.randint(2, 8))
elif seed == 9:
    h, w, rows = random_fp(rng.randint(2, 8), rng.randint(2, 8))
else:
    h, w, rows = from_key(rng.randint(2, 300), rng.randint(2, 300), rng.random())

print(h, w)
for row in rows:
    print(row)
