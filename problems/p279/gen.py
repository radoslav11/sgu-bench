"""Tests for p279 (Bipermutations): reconstruct an interleaving of a
permutation with its primed copy from the a(i) counts, n <= 1000.

YES instances are made by sampling a random bipermutation (random pi
plus a random/structured ballot interleaving of primes) and computing
its a-vector. Seeds 2-4 are maximal (n = 1000); seed 5 is a random
a-vector (almost surely NO), seed 6 perturbs a feasible vector, seed 7
is the statement sample, the rest are small random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def interleave(pi, style):
    n = len(pi)
    seq = []
    p = u = 0
    while len(seq) < 2 * n:
        can_prime = p < n
        can_unprimed = u < p
        if style == "primes_first":
            take_prime = can_prime
        elif style == "immediate":
            take_prime = can_prime and p == u
        else:
            take_prime = can_prime and (not can_unprimed or rng.random() < 0.5)
        if take_prime:
            seq.append(-pi[p])
            p += 1
        else:
            seq.append(pi[u])
            u += 1
    return seq


def a_vector(n, seq):
    pos_u = [0] * (n + 1)
    pos_p = [0] * (n + 1)
    for idx, v in enumerate(seq):
        if v > 0:
            pos_u[v] = idx
        else:
            pos_p[-v] = idx
    a = []
    for i in range(1, n + 1):
        cnt = sum(1 for j in range(i + 1, n + 1) if pos_u[i] < pos_u[j])
        cnt += sum(1 for j in range(1, i) if pos_u[i] < pos_p[j])
        a.append(cnt)
    return a


def emit(n, a):
    print(n)
    print(*a)


def random_pi(n):
    pi = list(range(1, n + 1))
    rng.shuffle(pi)
    return pi


if seed == 1:
    emit(1, [0])
elif seed == 2:
    n = 1000
    emit(n, a_vector(n, interleave(random_pi(n), "random")))
elif seed == 3:
    n = 1000
    emit(n, a_vector(n, interleave(random_pi(n), "immediate")))
elif seed == 4:
    emit(1000, [0] * 1000)
elif seed == 5:
    n = 1000
    emit(n, [rng.randint(0, n - 1) for _ in range(n)])
elif seed == 6:
    n = 1000
    a = a_vector(n, interleave(random_pi(n), "primes_first"))
    i = rng.randrange(n)
    a[i] = rng.randint(0, n - 1)
    emit(n, a)
elif seed == 7:
    emit(9, [2, 0, 3, 0, 4, 8, 1, 5, 4])
elif seed == 8:
    n = rng.randint(2, 60)
    emit(n, a_vector(n, interleave(random_pi(n), "random")))
elif seed == 9:
    n = rng.randint(2, 8)
    emit(n, [rng.randint(0, n - 1) for _ in range(n)])
else:
    n = rng.randint(2, 12)
    a = a_vector(n, interleave(random_pi(n), "random"))
    if rng.random() < 0.5:
        a[rng.randrange(n)] = rng.randint(0, n - 1)
    emit(n, a)
