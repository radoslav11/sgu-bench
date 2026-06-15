"""Tests for p343 (VaR): 1 <= T <= 10^4, 1 <= N <= 10, qty <= 1000,
prices in (0, 100000.00] with exactly 2 decimals.

Seed 1 is the smallest case (T=1, N=1). Seed 2 is T=10^4, N=10 with
random-walk prices (max input volume). Seed 3 is the same size but with
violently volatile prices spanning 0.01 .. 100000.00 (huge returns).
Later seeds include constant prices (zero variance), near-max prices,
penny prices, and random mid-size cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def fmt(x):
    cents = max(1, min(10000000, int(round(x * 100))))
    return f"{cents // 100}.{cents % 100:02d}"


def emit(T, N, walk="normal"):
    print(T, N)
    print(" ".join(str(rng.randint(1, 1000)) for _ in range(N)))
    cur = [rng.uniform(0.5, 100000.0) for _ in range(N)]
    for _ in range(T + 1):
        print(" ".join(fmt(x) for x in cur))
        for i in range(N):
            if walk == "normal":
                cur[i] *= rng.uniform(0.97, 1.03)
            elif walk == "wild":
                cur[i] = rng.uniform(0.01, 100000.0)
            elif walk == "constant":
                pass
            elif walk == "penny":
                cur[i] = rng.uniform(0.01, 0.30)
            cur[i] = min(100000.0, max(0.01, cur[i]))


if seed == 1:
    print(1, 1)
    print(1)
    print("1.00")
    print("1.00")
elif seed == 2:
    emit(10000, 10, "normal")
elif seed == 3:
    emit(10000, 10, "wild")
elif seed == 4:
    emit(10000, 10, "constant")
elif seed == 5:
    emit(10000, 1, "penny")
elif seed == 6:
    # Sample from the statement.
    print(5, 2)
    print(200, 10)
    for row in ["31.11 489.75", "31.04 488.04", "31.10 497.28",
                "31.15 504.28", "31.22 505.00", "30.69 501.02"]:
        print(row)
else:
    emit(rng.randint(1, 2000), rng.randint(1, 10),
         rng.choice(["normal", "wild", "penny"]))
