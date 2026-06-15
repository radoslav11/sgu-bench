"""Tests for p539 (Multiswap Sorting): 1 <= n <= 1000, |ai| <= 1e9.

Seed 1 is the minimal single-element array. Seeds 2 and 3 are maximal
n = 1000 arrays with different structure (a reversed permutation needing
two steps, and a heavily duplicated array). Later seeds cover already
sorted input, one-step-solvable reversals, and random arrays with and
without duplicates.
"""

import random
import sys

LIM = 10 ** 9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(a):
    print(len(a))
    print(" ".join(map(str, a)))


if seed == 1:
    emit([rng.randint(-LIM, LIM)])
    sys.exit()

if seed == 2:
    # Reversed distinct permutation of size 1000: forces a long cycle,
    # answer is 2 steps.
    a = list(range(1, 1001))
    rng.shuffle(a)
    a = sorted(a, reverse=True)
    emit(a)
    sys.exit()

if seed == 3:
    # Maximal size with many duplicates, randomly shuffled.
    a = [rng.randint(0, 5) for _ in range(1000)]
    rng.shuffle(a)
    emit(a)
    sys.exit()

if seed == 4:
    # Already sorted -> 0 steps.
    a = sorted(rng.randint(-LIM, LIM) for _ in range(1000))
    emit(a)
    sys.exit()

if seed == 5:
    # Pure reversal of distinct values -> solvable in one step.
    a = sorted((rng.randint(-LIM, LIM) for _ in range(1000)), reverse=True)
    while len(set(a)) != len(a):
        a = sorted((rng.randint(-LIM, LIM) for _ in range(1000)), reverse=True)
    emit(a)
    sys.exit()

# Random mid/large arrays; vary size and value range / duplicate density.
n = rng.randint(2, 1000)
if seed % 2 == 0:
    a = [rng.randint(-LIM, LIM) for _ in range(n)]
else:
    a = [rng.randint(-3, 3) for _ in range(n)]
emit(a)
