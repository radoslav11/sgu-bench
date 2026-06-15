"""Tests for p304 (Mars Stomatology): max teeth cured within budget P.

N <= 600 teeth, K <= N gums, tooth costs A and gum anesthesia costs B
in 1..600, P <= 10^6. Budgets are drawn around a fraction of the total
cost so the knapsack boundary is actually binding. Seed 2 is maximal
with K = N (every tooth on its own gum), seed 3 maximal with few gums
and many ties.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def gen(n, k, p, alo=1, ahi=600, blo=1, bhi=600):
    print(n, k, p)
    print(*[rng.randint(blo, bhi) for _ in range(k)])
    # every gum gets at least one tooth via a shuffled cover, the rest
    # of the teeth land on random gums
    gum_of = list(range(1, k + 1)) + [rng.randint(1, k) for _ in range(n - k)]
    rng.shuffle(gum_of)
    for c in gum_of:
        print(rng.randint(alo, ahi), c)


if seed == 1:
    gen(1, 1, 1)  # minimal; cheapest cure costs >= 2 so S = 0
elif seed == 2:
    gen(600, 600, rng.randint(100000, 400000))  # max N, gum per tooth
elif seed == 3:
    gen(600, 3, rng.randint(50000, 150000), alo=1, ahi=4)  # heavy ties
elif seed == 4:
    gen(600, 600, 1000000)  # everything affordable: 720000 max total
elif seed == 5:
    gen(600, 30, rng.randint(1, 600))  # tiny budget
elif seed == 6:
    gen(600, 60, rng.randint(60000, 220000), blo=600, bhi=600)
elif seed == 7:
    gen(rng.randint(2, 20), rng.randint(1, 5), rng.randint(2, 2000))
elif seed == 8:
    n = rng.randint(50, 300)
    gen(n, rng.randint(1, n), rng.randint(1000, 90000))
elif seed == 9:
    gen(600, 1, rng.randint(1000, 180000))  # single gum
else:
    n = rng.randint(300, 600)
    gen(n, rng.randint(2, n), rng.randint(10000, 200000), alo=1, ahi=10)
