"""Tests for p242 (Student's Morning): 0 <= K <= N <= 200.

Input: N K, then N preference lists "cnt u1 ... ucnt" with universities
numbered 1..K. Seed 1 is the minimal empty case, seeds 2-3 are maximal
(dense YES and dense NO), the rest mix tight-feasibility, K=0 and random
sparse/dense structures.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, k, lists):
    print(n, k)
    for lst in lists:
        print(len(lst), *lst)


if seed == 1:
    # smallest valid input: no students, no universities
    print(0, 0)
elif seed == 2:
    # maximal YES: 200 students, 100 universities, full preference lists
    n, k = 200, 100
    emit(n, k, [list(range(1, k + 1)) for _ in range(n)])
elif seed == 3:
    # maximal NO: K = N, full lists, 2K = 400 slots but only 200 students
    n, k = 200, 200
    emit(n, k, [list(range(1, k + 1)) for _ in range(n)])
elif seed == 4:
    # K = 0 with many students: trivially YES, lists are empty
    emit(200, 0, [[] for _ in range(200)])
elif seed == 5:
    # barely feasible: students 2j-1, 2j accept university j only
    n, k = 200, 100
    lists = [[i // 2 + 1] for i in range(n)]
    emit(n, k, lists)
elif seed == 6:
    # barely infeasible: like seed 5 but university k loses one candidate
    n, k = 200, 100
    lists = [[i // 2 + 1] for i in range(n)]
    lists[n - 1] = []
    emit(n, k, lists)
else:
    if seed == 7:
        n, k = 50, 20
        prob = 0.3
    elif seed == 8:
        n, k = 200, 50
        prob = 0.02
    elif seed == 9:
        n, k = 199, 99
        prob = 0.5
    else:
        n = rng.randint(1, 200)
        k = rng.randint(0, n)
        prob = rng.choice([0.05, 0.2, 0.8])
    lists = []
    for _ in range(n):
        lst = [u for u in range(1, k + 1) if rng.random() < prob]
        lists.append(lst)
    emit(n, k, lists)
