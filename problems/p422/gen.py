"""Tests for p422 (Fast Typing): 1 <= n <= 3000, 1 <= t <= 10^6,
0 <= a_i < 1 mistype probabilities (kept <= 0.95 for numeric stability).

Seed 1 is minimal, seeds 2-3 are maximal n with different probability
profiles, later seeds mix sizes, t values and adversarial patterns
(all-zero probabilities, near-uniform high probabilities, spikes).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, t, probs):
    print(n, t)
    print(" ".join(f"{p:.6f}" for p in probs))


if seed == 1:
    emit(1, 1, [0.0])
elif seed == 2:
    n, t = 3000, 1000000
    emit(n, t, [rng.uniform(0.0, 0.95) for _ in range(n)])
elif seed == 3:
    n, t = 3000, 1
    probs = []
    for i in range(n):
        if i % 7 == 0:
            probs.append(rng.uniform(0.8, 0.95))
        else:
            probs.append(rng.uniform(0.0, 0.05))
    emit(n, t, probs)
elif seed == 4:
    n = 3000
    emit(n, 1000, [0.0] * n)
elif seed == 5:
    n = 3000
    emit(n, 500000, [0.95] * n)
elif seed == 6:
    n = rng.randint(2, 50)
    emit(n, rng.randint(1, 1000000), [rng.uniform(0.0, 0.9) for _ in range(n)])
elif seed == 7:
    n = rng.randint(500, 1500)
    probs = [rng.choice([0.0, 0.5, 0.9]) for _ in range(n)]
    emit(n, rng.randint(1, 100), probs)
elif seed == 8:
    n = rng.randint(1000, 3000)
    probs = [0.0] * n
    for i in rng.sample(range(n), 5):
        probs[i] = 0.95
    emit(n, rng.randint(100000, 1000000), probs)
elif seed == 9:
    n = rng.randint(100, 400)
    probs = [min(0.95, rng.expovariate(20.0)) for _ in range(n)]
    emit(n, rng.randint(1, 10), probs)
else:
    n = rng.randint(2000, 3000)
    emit(n, rng.randint(1, 1000000),
         [rng.uniform(0.0, 0.95) for _ in range(n)])
