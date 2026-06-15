"""Tests for p255 (Winsock 3 Beta): N <= 100 queries, 0 <= M <= 2^31-1.

Seed 1 is the minimal input (N=0). Seeds 2-3 are maximal (N=100) with
adversarial M values: triangular+1 numbers (YES), their off-by-one
neighbours, tiny values 0/1/2 and the 2^31-1 boundary.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXM = 2**31 - 1

triangular_plus_1 = []
n = 2
while n * (n - 1) // 2 + 1 <= MAXM:
    triangular_plus_1.append(n * (n - 1) // 2 + 1)
    n += 1

if seed == 1:
    print(0)
    sys.exit()

if seed == 2:
    vals = [0, 1, 2, MAXM, MAXM - 1, triangular_plus_1[-1]]
    while len(vals) < 100:
        vals.append(rng.randint(0, MAXM))
    rng.shuffle(vals)
elif seed == 3:
    vals = []
    while len(vals) < 100:
        t = rng.choice(triangular_plus_1)
        vals.append(t + rng.choice([-1, 0, 0, 1]))
    vals = [min(max(v, 0), MAXM) for v in vals]
else:
    cnt = rng.randint(1, 100)
    vals = []
    for _ in range(cnt):
        kind = rng.random()
        if kind < 0.4:
            vals.append(rng.randint(0, MAXM))
        elif kind < 0.7:
            vals.append(rng.choice(triangular_plus_1))
        else:
            vals.append(rng.randint(0, 50))

print(len(vals))
for v in vals:
    print(v)
