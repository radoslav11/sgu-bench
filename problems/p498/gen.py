"""Tests for p498 (Coins): posterior probability that p < q.

Input: T test cases, each "n1 m1 n2 m2" with 1 <= n1, n2 <= 1000,
0 <= m1 <= min(n1, 50), 0 <= m2 <= min(n2, 50). The reference work per
case is O(n1), so seeds 2-3 use T at the maximum with n1 maxed out and
two different (m1, m2) regimes.

Bounds used here: T <= 10000.
"""

import random
import sys

T_MAX = 10000
N_MAX = 1000
M_MAX = 50

seed = int(sys.argv[1])
rng = random.Random(seed)


def case(n1, m1, n2, m2):
    return f"{n1} {m1} {n2} {m2}"


def emit(cases):
    sys.stdout.write(str(len(cases)) + "\n")
    sys.stdout.write("\n".join(cases) + "\n")


if seed == 1:
    # Minimal: single test at the smallest dimensions.
    emit([case(1, 0, 1, 0)])

elif seed == 2:
    # Maximal: T at the limit, every case n1 = n2 = 1000 with the longest
    # summation loop and m1 = m2 = 50 (heaviest binomial prefixes).
    emit([case(N_MAX, M_MAX, N_MAX, M_MAX) for _ in range(T_MAX)])

elif seed == 3:
    # Second maximal: T at the limit, n1 = n2 = 1000 but randomised m1, m2
    # over their whole range so summation terms vary in magnitude.
    cases = [
        case(N_MAX, rng.randint(0, M_MAX), N_MAX, rng.randint(0, M_MAX))
        for _ in range(T_MAX)
    ]
    emit(cases)

elif seed == 4:
    # Boundary mix: m equal to 0 or to n (clamped to 50), extreme posteriors.
    cases = []
    for _ in range(T_MAX):
        n1 = rng.randint(1, N_MAX)
        n2 = rng.randint(1, N_MAX)
        m1 = rng.choice([0, min(n1, M_MAX)])
        m2 = rng.choice([0, min(n2, M_MAX)])
        cases.append(case(n1, m1, n2, m2))
    emit(cases)

elif seed == 5:
    # Symmetric cases (identical observations) where the answer is exactly
    # 0.5, good for precision checking.
    cases = []
    for _ in range(T_MAX):
        n = rng.randint(1, N_MAX)
        m = rng.randint(0, min(n, M_MAX))
        cases.append(case(n, m, n, m))
    emit(cases)

elif seed == 6:
    # Small n with full m range, dense around small dimensions.
    cases = []
    for _ in range(T_MAX):
        n1 = rng.randint(1, 50)
        n2 = rng.randint(1, 50)
        cases.append(
            case(n1, rng.randint(0, min(n1, M_MAX)), n2, rng.randint(0, min(n2, M_MAX)))
        )
    emit(cases)

else:
    # General randomised mix.
    cases = []
    t = rng.randint(1, T_MAX)
    for _ in range(t):
        n1 = rng.randint(1, N_MAX)
        n2 = rng.randint(1, N_MAX)
        m1 = rng.randint(0, min(n1, M_MAX))
        m2 = rng.randint(0, min(n2, M_MAX))
        cases.append(case(n1, m1, n2, m2))
    emit(cases)
