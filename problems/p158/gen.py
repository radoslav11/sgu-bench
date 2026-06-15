"""Tests for p158 (Commuter Train): 0 < L <= 5000, 0 < M <= 300
passengers (sorted, may repeat), 0 < N <= 300 doors (strict, D1 = 0).

Seed 1 is the minimal L=1, M=N=1 case. Seed 2 is fully maximal with
random spread; seed 3 is maximal with passengers crowded at the
platform ends and doors packed tightly (large plateau of optima).
Seed 4 puts every passenger on one point, seed 5 uses a single door,
seed 6 evenly spaced doors against clustered passengers; the rest are
random mid-size mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(l, p, d):
    print(l)
    print(len(p))
    print(" ".join(map(str, sorted(p))))
    print(len(d))
    rest = sorted(d)[1:]
    if rest:
        print(" ".join(map(str, rest)))


if seed == 1:
    emit(1, [0], [0])
elif seed == 2:
    l = 5000
    p = [rng.randint(0, l) for _ in range(300)]
    d = [0] + rng.sample(range(1, l + 1), 299)
    emit(l, p, d)
elif seed == 3:
    l = 5000
    p = [rng.randint(0, 80) for _ in range(150)]
    p += [rng.randint(l - 80, l) for _ in range(150)]
    d = [0] + rng.sample(range(1, 400), 299)
    emit(l, p, d)
elif seed == 4:
    l = 5000
    x = rng.randint(0, l)
    p = [x] * 300
    d = [0] + rng.sample(range(1, l + 1), 299)
    emit(l, p, d)
elif seed == 5:
    l = 5000
    p = [rng.randint(0, l) for _ in range(300)]
    emit(l, p, [0])
elif seed == 6:
    l = 4998
    d = [i * (l // 600) for i in range(300)]
    p = [rng.randint(2000, 2300) for _ in range(300)]
    emit(l, p, d)
else:
    l = rng.randint(1, 5000)
    m = rng.randint(1, 300)
    n = rng.randint(1, min(300, l + 1))
    p = [rng.randint(0, l) for _ in range(m)]
    dn = rng.randint(n - 1, l)
    d = [0] + (rng.sample(range(1, dn + 1), n - 1) if n > 1 else [])
    emit(l, p, d)
