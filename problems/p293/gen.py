"""Tests for p293 (Game with Q an C): n <= 2005, string of 2n-1 Q/C letters.

Seed 1 is the minimal case, seeds 2-3 are maximal (random letters and
heavily skewed letters), the rest mix random sizes with adversarial
patterns: single-letter strings, alternating letters, and long runs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_N = 2005


def emit(n, letters):
    assert len(letters) == 2 * n - 1
    print(n)
    print("".join(letters))


if seed == 1:
    emit(1, "Q")
elif seed == 2:
    emit(MAX_N, [rng.choice("QC") for _ in range(2 * MAX_N - 1)])
elif seed == 3:
    # maximal size, heavily skewed distribution with long runs
    letters = []
    while len(letters) < 2 * MAX_N - 1:
        ch = "Q" if rng.random() < 0.9 else "C"
        letters.extend(ch * rng.randint(1, 50))
    emit(MAX_N, letters[: 2 * MAX_N - 1])
elif seed == 4:
    emit(MAX_N, "Q" * (2 * MAX_N - 1))
elif seed == 5:
    letters = ["QC"[i % 2] for i in range(2 * MAX_N - 1)]
    emit(MAX_N, letters)
elif seed == 6:
    # half Q's then half C's
    m = 2 * MAX_N - 1
    emit(MAX_N, "Q" * (m // 2) + "C" * (m - m // 2))
else:
    n = rng.randint(1, MAX_N)
    p = rng.random()
    emit(n, ["Q" if rng.random() < p else "C" for _ in range(2 * n - 1)])
