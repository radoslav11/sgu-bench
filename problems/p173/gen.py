"""Tests for p173 (Coins): 2<=N<=50, 1<=M<=10, 1<=L<=200, 2<=K<=N,
Si<=N-K+1, Di<=10^6.

A hidden vector A (K-1 bits) defines transformation X; the L
(before, after) pairs must determine A uniquely, so the generated
before-rows always include the K-1 unit coefficient vectors. The final
row can be arbitrary because X is a bijection on K-bit blocks.

Seed 1 is minimal, seeds 2-3 maximal (M=10, every Di=10^6, L=200) with
K=N=50 resp. K=25, seeds 4+ random mid-size including A=0 / A=all-ones.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def apply_x(c, a, k):
    # cyclic left shift: new position j holds old position j+1 (bit i of
    # the mask is row position i), then conditionally flip the last coin
    s = (c >> 1) | ((c & 1) << (k - 1))
    parity = ((s & a) & ((1 << (k - 1)) - 1)).bit_count() & 1
    return s ^ (parity << (k - 1))


if seed == 1:
    n, m, k, l = 2, 1, 2, 1
elif seed == 2:
    n, m, k, l = 50, 10, 50, 200
elif seed == 3:
    n, m, k, l = 50, 10, 25, 200
else:
    n = rng.randint(2, 50)
    k = rng.randint(2, n)
    m = rng.randint(1, 10)
    l = rng.randint(k - 1 if k > 2 else 1, 200)

if seed == 4:
    a = 0
elif seed == 5:
    a = (1 << (k - 1)) - 1
else:
    a = rng.getrandbits(k - 1)

ops = []
for _ in range(m):
    s = rng.randint(1, n - k + 1)
    if seed == 1:
        d = 1
    elif seed in (2, 3):
        d = 10**6
    else:
        d = rng.randint(1, 10**6 if seed % 2 else 50)
    ops.append((s, d))

befores = [1 << (i + 1) for i in range(k - 1)]  # guarantee unique A
while len(befores) < l:
    befores.append(rng.getrandbits(k))
rng.shuffle(befores)

final_row = rng.getrandbits(n)

print(n, m, k, l)
print(" ".join(f"{s} {d}" for s, d in ops))
for b in befores:
    after = apply_x(b, a, k)
    print("".join("1" if (b >> i) & 1 else "0" for i in range(k)))
    print("".join("1" if (after >> i) & 1 else "0" for i in range(k)))
print("".join("1" if (final_row >> i) & 1 else "0" for i in range(n)))
