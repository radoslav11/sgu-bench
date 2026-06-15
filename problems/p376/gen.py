"""Tests for p376 (Berland All-Round Competitions).

N, K <= 1000; W, R, P in [1, 10^6] with all strengths P distinct;
A_i, B_j in [1, 1000]. Seed 1 minimal, seed 2 maximal random, seed 3
maximal with heavy ties (equal swim skills and equal horse values force
strength tie-breaking everywhere), later seeds mid-size adversarial.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    n, k = 1, 1
elif seed in (2, 3):
    n, k = 1000, 1000
else:
    n, k = rng.randint(1, 300), rng.randint(1, 300)

strengths = rng.sample(range(1, 10**6 + 1), n)

if seed == 3:
    # everyone swims identically and rides identically; all horses of a
    # stage are equal too, so every comparison is decided by strength
    w_vals = [500000] * n
    r_vals = [123456] * n
    a_vals = [1000] * k
    b_vals = [1000] * n
elif seed % 2 == 0:
    w_vals = [rng.randint(1, 10**6) for _ in range(n)]
    r_vals = [rng.randint(1, 10**6) for _ in range(n)]
    a_vals = [rng.randint(1, 1000) for _ in range(k)]
    b_vals = [rng.randint(1, 1000) for _ in range(n)]
else:
    # small value pools create many arrival-time collisions mid-race
    w_vals = [rng.choice([1, 2, 10**6]) for _ in range(n)]
    r_vals = [rng.choice([1, 5, 10**6]) for _ in range(n)]
    a_vals = [rng.choice([1, 2, 1000]) for _ in range(k)]
    b_vals = [rng.choice([1, 3, 1000]) for _ in range(n)]

print(n, k)
for i in range(n):
    print(w_vals[i], r_vals[i], strengths[i])
print(" ".join(map(str, a_vals)))
print(" ".join(map(str, b_vals)))
