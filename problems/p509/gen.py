"""Tests for p509 (Chameleons All Around).

n chameleons on a circle of length L, positions distinct in [0, L), colors
in [1, 1e9], directions +-1, evolved to time T in [0, 1e18]. No meeting may
happen at exactly T. To guarantee this we make L and all positions multiples
of 4 and keep T odd. Two opposite-moving chameleons meet at times of the form
gap/2 + k*(L/2): with gap and L divisible by 4 these are all even, so an odd
T can never coincide with a meeting.

Seed 1 is the minimal case (n = 1). Seed 2 is maximal (n = 100000, L = 1e9,
T near 1e18, mixed directions). Seed 3 is a second max-size test where nearly
every chameleon moves counter-clockwise so the color binary search is
exercised on the full set. Later seeds randomize size and direction mix, plus
an all-same direction stress and a tightly packed positions case.
"""

import random
import sys

NMAX = 100000
LMAX = 10**9
CMAX = 10**9
TMAX = 10**18

seed = int(sys.argv[1])
rng = random.Random(seed)


def mul4_down(x):
    return x - (x & 3)


def emit(n, L, chams, T):
    # chams: list of (p, c, d) with p distinct, p % 4 == 0, 0 <= p < L
    out = [str(n), str(L)]
    for p, c, d in chams:
        out.append(f"{p} {c} {d}")
    out.append(str(T))
    sys.stdout.write("\n".join(out) + "\n")


def random_positions(n, L, rng):
    # distinct positions that are multiples of 4 in [0, L)
    slots = L // 4  # number of multiple-of-4 values in [0, L)
    picks = rng.sample(range(slots), n)
    return [4 * s for s in picks]


# L choices are all multiples of 4.
BIG_L = LMAX - (LMAX & 3)  # 10**9 -> 999999996, multiple of 4

if seed == 1:
    # minimal: single chameleon
    L = 1000000
    p = mul4_down(rng.randint(0, L - 1))
    c = rng.randint(1, CMAX)
    d = rng.choice([-1, 1])
    T = 999999999999999999  # odd, near max
    emit(1, L, [(p, c, d)], T)
    sys.exit()

if seed == 2:
    # maximal: full n, big L, mixed directions, T near 1e18 (odd)
    n = NMAX
    L = BIG_L
    pos = random_positions(n, L, rng)
    chams = [(p, rng.randint(1, CMAX), rng.choice([-1, 1])) for p in pos]
    T = TMAX - 1  # 999...999, odd
    emit(n, L, chams, T)
    sys.exit()

if seed == 3:
    # second max-size: almost all counter-clockwise, a few clockwise
    n = NMAX
    L = BIG_L
    pos = random_positions(n, L, rng)
    chams = []
    for i, p in enumerate(pos):
        d = 1 if i % 50 == 0 else -1
        chams.append((p, rng.randint(1, CMAX), d))
    T = TMAX - 1
    emit(n, L, chams, T)
    sys.exit()

if seed == 4:
    # all same direction (clockwise): no color changes ever
    n = NMAX
    L = BIG_L
    pos = random_positions(n, L, rng)
    chams = [(p, rng.randint(1, CMAX), 1) for p in pos]
    T = (rng.randint(0, TMAX) | 1)
    emit(n, L, chams, T)
    sys.exit()

if seed == 5:
    # tightly packed: positions 0,4,8,... in a small circle, big T
    n = 50000
    L = 4 * n + 4  # multiple of 4, just enough room
    pos = [4 * i for i in range(n)]
    rng.shuffle(pos)
    chams = [(p, rng.randint(1, CMAX), rng.choice([-1, 1])) for p in pos]
    T = TMAX - 1
    emit(n, L, chams, T)
    sys.exit()

# seeds 6+: randomized mid-size tests
n = rng.randint(2, 2000)
L = mul4_down(rng.randint(4 * n, BIG_L))
if L < 4 * n:
    L = 4 * n
pos = random_positions(n, L, rng)
chams = [(p, rng.randint(1, CMAX), rng.choice([-1, 1])) for p in pos]
T = (rng.randint(0, TMAX) | 1)
emit(n, L, chams, T)
