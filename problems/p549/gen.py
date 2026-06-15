"""Tests for p549 (Dumbbells): 1 <= n,k <= 4000; 1 <= m_i,c_i <= 4000.

- seed 1: minimal input (n = k = 1).

- seed 2: maximal n with few distinct masses so every mass has a huge
  supply (forces large t and large per-mass cost sums).

- seed 3: maximal n with k near the limit and masses spread across the
  full range so counts hover near k (stresses the threshold selection).

- seeds 4+: random mid/large cases plus adversarial layouts (single mass,
  k larger than distinct masses, many ties in counts and costs).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 4000


def emit(n, k, dumbbells):
    out = [f"{n} {k}"]
    for m, c in dumbbells:
        out.append(f"{m} {c}")
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, 1, [(1, 1)])

elif seed == 2:
    # n = 4000, only k distinct masses -> each mass has ~4000/k copies.
    n = LIM
    k = 20
    masses = list(range(1, k + 1))
    dumbbells = []
    for i in range(n):
        m = masses[i % k]
        c = rng.randint(1, LIM)
        dumbbells.append((m, c))
    rng.shuffle(dumbbells)
    emit(n, k, dumbbells)

elif seed == 3:
    # n = 4000, k = 1000, masses spread so counts cluster near the limit.
    n = LIM
    k = 1000
    dumbbells = []
    for i in range(n):
        m = rng.randint(1, LIM)
        c = rng.randint(1, LIM)
        dumbbells.append((m, c))
    emit(n, k, dumbbells)

elif seed == 4:
    # Single mass, k = 1: one set per dumbbell? No, t = count, k=1.
    n = LIM
    k = 1
    dumbbells = [(7, rng.randint(1, LIM)) for _ in range(n)]
    emit(n, k, dumbbells)

elif seed == 5:
    # k larger than number of distinct masses -> answer 0 0.
    n = LIM
    k = 4000
    dumbbells = [(rng.randint(1, 100), rng.randint(1, LIM)) for _ in range(n)]
    emit(n, k, dumbbells)

elif seed == 6:
    # Many equal costs to stress tie handling.
    n = LIM
    k = rng.randint(2, 50)
    distinct = rng.randint(k, k + 30)
    masses = rng.sample(range(1, LIM + 1), distinct)
    dumbbells = []
    for i in range(n):
        m = rng.choice(masses)
        dumbbells.append((m, rng.choice([1, 2, LIM])))
    emit(n, k, dumbbells)

elif seed == 7:
    # k = n, every dumbbell distinct mass -> t = 1 if exactly n masses.
    n = rng.randint(1, 4000)
    k = n
    masses = rng.sample(range(1, LIM + 1), min(n, LIM))
    if len(masses) < n:
        masses = masses + [rng.randint(1, LIM) for _ in range(n - len(masses))]
    dumbbells = [(masses[i], rng.randint(1, LIM)) for i in range(n)]
    emit(n, k, dumbbells)

else:
    n = rng.randint(1, 4000)
    k = rng.randint(1, 4000)
    distinct = rng.randint(1, max(1, min(n, 200)))
    masses = rng.sample(range(1, LIM + 1), min(distinct, LIM))
    dumbbells = []
    for _ in range(n):
        m = rng.choice(masses)
        c = rng.randint(1, LIM)
        dumbbells.append((m, c))
    emit(n, k, dumbbells)
