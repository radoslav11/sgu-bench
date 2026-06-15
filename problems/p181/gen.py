"""Tests for p181 (X-Sequence): k-th term of x_i = (a*x^2+b*x+c) mod M.

Input is one line: A alpha beta gamma M k with 1<=A<=10000,
0<=alpha,beta,gamma<=100, 1<=M<=1000, 0<=k<=10^9. Seed 1 is minimal,
seeds 2-3 use every bound at its maximum, later seeds probe k=0 with
A > M, M=1, fixed points and random parameters.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0, 0, 0, 1, 0)
elif seed == 2:
    print(10000, 100, 100, 100, 1000, 10**9)
elif seed == 3:
    # max k with a linear map (alpha=0) and max modulus
    print(9999, 0, 100, 99, 1000, 10**9)
elif seed == 4:
    # k=0 must print A itself even though A > M
    print(10000, 100, 100, 100, 3, 0)
elif seed == 5:
    # M=1: everything collapses to 0 for k >= 1
    print(537, 42, 17, 99, 1, 999999937)
elif seed == 6:
    # constant map (alpha=beta=0)
    print(123, 0, 0, 77, 100, 10**9)
else:
    A = rng.randint(1, 10000)
    alpha = rng.randint(0, 100)
    beta = rng.randint(0, 100)
    gamma = rng.randint(0, 100)
    M = rng.randint(1, 1000)
    k = rng.choice([rng.randint(0, 1000), rng.randint(0, 10**9)])
    print(A, alpha, beta, gamma, M, k)
