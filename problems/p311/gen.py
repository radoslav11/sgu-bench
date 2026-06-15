"""Tests for p311 (Ice-cream Tycoon): up to 1e5 ARRIVE/BUY queries,
1 <= n, c <= 1e6, 1 <= t <= 1e12.

Seed 1 is the single-query minimum. Seed 2 is a full-size random mix of
big deliveries and buys. Seed 3 is full-size with 5e4 distinct-price
deliveries followed by 5e4 mostly-failing buys (stresses structures
that rescan the inventory per query). Later seeds use a small price
palette and exact-budget boundary buys (t equals the cost of the n
cheapest pieces, or that cost minus one).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10**6
MAXT = 10**12
MAXQ = 10**5


def boundary_test(q, prices, big_n):
    # Simulate inventory over a small palette of distinct prices so the
    # exact cost of the n cheapest pieces is cheap to compute.
    inv = {}
    lines = []
    for _ in range(q):
        if rng.random() < 0.55 or not inv:
            c = rng.choice(prices)
            n = rng.randint(1, big_n)
            inv[c] = inv.get(c, 0) + n
            lines.append(f"ARRIVE {n} {c}")
        else:
            total = sum(inv.values())
            n = rng.randint(1, min(MAXV, max(1, total + rng.randint(-2, 2))))
            cost = 0
            left = n
            for c in sorted(inv):
                take = min(left, inv[c])
                cost += take * c
                left -= take
                if left == 0:
                    break
            if left > 0 or cost > MAXT:
                t = rng.randint(1, MAXT)
            else:
                t = max(1, min(MAXT, cost - rng.choice([0, 0, 0, 1, -1])))
            if t >= cost and left == 0:
                rem = n
                for c in sorted(inv):
                    take = min(rem, inv[c])
                    inv[c] -= take
                    if inv[c] == 0:
                        del inv[c]
                    rem -= take
                    if rem == 0:
                        break
            lines.append(f"BUY {n} {t}")
    print("\n".join(lines))


if seed == 1:
    print("BUY 1 1")
elif seed == 2:
    lines = []
    for _ in range(MAXQ):
        if rng.random() < 0.5:
            lines.append(f"ARRIVE {rng.randint(1, MAXV)} {rng.randint(1, MAXV)}")
        else:
            lines.append(f"BUY {rng.randint(1, MAXV)} {rng.randint(1, MAXT)}")
    print("\n".join(lines))
elif seed == 3:
    lines = []
    half = MAXQ // 2
    prices = rng.sample(range(1, MAXV + 1), half)
    for c in prices:
        lines.append(f"ARRIVE {rng.randint(MAXV // 2, MAXV)} {c}")
    for i in range(half):
        if i % 10 == 9:
            lines.append(f"BUY {rng.randint(1, 1000)} {MAXT}")
        else:
            lines.append(f"BUY {rng.randint(MAXV // 2, MAXV)} 1")
    print("\n".join(lines))
elif seed == 4:
    boundary_test(MAXQ, rng.sample(range(1, MAXV + 1), 40), MAXV)
elif seed == 5:
    boundary_test(2000, rng.sample(range(1, 100), 10), 5)
elif seed == 6:
    boundary_test(5000, [1, 2, MAXV - 1, MAXV], MAXV)
elif seed == 7:
    boundary_test(MAXQ, rng.sample(range(1, MAXV + 1), 200), 1000)
elif seed == 8:
    # everything at one price: partial-group splits dominate
    boundary_test(MAXQ, [123456], MAXV)
else:
    q = rng.randint(1, 300)
    lines = []
    for _ in range(q):
        if rng.random() < 0.4:
            lines.append(f"ARRIVE {rng.randint(1, 10)} {rng.randint(1, 5)}")
        else:
            lines.append(f"BUY {rng.randint(1, 15)} {rng.randint(1, 60)}")
    print("\n".join(lines))
