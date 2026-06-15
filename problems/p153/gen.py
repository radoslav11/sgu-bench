"""Tests for p153 (misere subtraction game, who wins).

Each case: heap of N <= 10^9 matches, allowed takes {1, P1..Pm} with
2 <= Pi <= 9, 0 <= m <= 8. K cases per file (K = 100 on the big seeds).
The second line of a case lists the Pi values and is empty when m = 0.
Adversarial cases: N = 10^9 with every possible move-set size, tiny N,
single-move sets {p} whose dp is periodic with period p + 1.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def case_lines(n, ps):
    yield f"{n} {len(ps)}"
    yield " ".join(map(str, ps))


def emit(cases):
    print(len(cases))
    for n, ps in cases:
        for line in case_lines(n, ps):
            print(line)


if seed == 1:
    emit([(1, [])])  # minimal: one match, only move is taking it -> lose
elif seed == 2:
    # maximal: 100 cases, all N = 10^9, every subset size of {2..9}
    cases = []
    for i in range(100):
        m = i % 9
        ps = sorted(rng.sample(range(2, 10), m))
        cases.append((10**9, ps))
    emit(cases)
elif seed == 3:
    # maximal, different structure: huge N values, skewed move sets
    cases = []
    for i in range(100):
        n = rng.randint(10**9 - 1000, 10**9)
        if i % 3 == 0:
            ps = [rng.randint(2, 9)]  # single extra move, period p+1
        elif i % 3 == 1:
            ps = list(range(2, 10))  # full move set
        else:
            ps = sorted(rng.sample(range(2, 10), rng.randint(0, 8)))
        cases.append((n, ps))
    emit(cases)
elif seed == 4:
    emit([(5, [2, 3, 5])])  # the statement sample
elif seed == 5:
    # all tiny heaps: N in 1..20 with assorted move sets
    cases = [(n, sorted(rng.sample(range(2, 10), rng.randint(0, 8))))
             for n in range(1, 21)]
    emit(cases)
else:
    k = rng.randint(1, 100)
    cases = []
    for _ in range(k):
        n = rng.choice([
            rng.randint(1, 30),
            rng.randint(1, 10**4),
            rng.randint(1, 10**9),
        ])
        ps = sorted(rng.sample(range(2, 10), rng.randint(0, 8)))
        cases.append((n, ps))
    emit(cases)
