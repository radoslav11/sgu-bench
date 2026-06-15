"""Tests for p366 (Computer Game). N up to 60000, K up to min(N, 20),
values a, b in 0..50. Pick K opponents minimizing |A - B|, then
maximizing A + B.

Seed plan:
- seed 1: N = 1, K = 1 (minimal).
- seed 2: N = 60000, K = 20, uniform random values (maximal).
- seed 3: N = 60000, K = 20, polarized values: only (50, 0) and (0, 50)
  plus a few odd-parity entries so |A - B| = 0 needs balanced picking.
- seed 4: N = 60000, K = 20, all pairs equal a = b (diff always 0, pure
  A + B maximization with massive ties).
- seed 5: N = 60000, K = 19 (odd K), values forcing nonzero |A - B|.
- seed 6: all zeros.
- seeds 7+: random N, K, mixed distributions.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(k, pairs):
    out = [f"{len(pairs)} {k}"]
    out += [f"{a} {b}" for a, b in pairs]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, [(7, 3)])
elif seed == 2:
    emit(20, [(rng.randint(0, 50), rng.randint(0, 50)) for _ in range(60000)])
elif seed == 3:
    pairs = []
    for _ in range(59990):
        pairs.append((50, 0) if rng.random() < 0.5 else (0, 50))
    for _ in range(10):
        pairs.append((rng.randint(0, 50), rng.randint(0, 49)))
    rng.shuffle(pairs)
    emit(20, pairs)
elif seed == 4:
    pairs = []
    for _ in range(60000):
        v = rng.randint(0, 50)
        pairs.append((v, v))
    emit(20, pairs)
elif seed == 5:
    # Every a - b is odd, so with K = 19 the difference is odd too.
    pairs = []
    for _ in range(60000):
        a = rng.randint(0, 49)
        b = a + 1 if rng.random() < 0.5 else max(0, a - 1)
        if (a - b) % 2 == 0:
            b = a + 1
        pairs.append((a, b))
    emit(19, pairs)
elif seed == 6:
    emit(20, [(0, 0)] * 1000)
else:
    n = rng.randint(1, 60000)
    k = rng.randint(1, min(n, 20))
    style = rng.random()
    pairs = []
    for _ in range(n):
        if style < 0.3:
            pairs.append((rng.randint(0, 5), rng.randint(45, 50)))
        elif style < 0.6:
            pairs.append((rng.randint(0, 50), rng.randint(0, 50)))
        else:
            a = rng.randint(0, 50)
            pairs.append((a, min(50, max(0, a + rng.randint(-2, 2)))))
    emit(k, pairs)
