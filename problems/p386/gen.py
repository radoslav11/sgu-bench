"""Tests for p386 (Happy Birthday, Jedi Knight!).

Input: n and a prime p, then an n x n integer matrix with entries in
[0, p-1] whose rows are linearly independent (nonzero exact determinant).

Seed 1 is the minimal 2x2 identity, seed 2 a dense random 50x50 matrix
with a ~1e9 prime, seed 3 a sparse permutation-plus-noise 50x50 matrix.
Seed 4 is adversarial: a tiny prime p=3 and a matrix that is singular
mod p yet independent over the integers, so the answer is 0. Remaining
seeds are random sizes and primes; every matrix is certified by an exact
Bareiss determinant before being printed.
"""

import random
import sys


def bareiss_det(matrix):
    a = [row[:] for row in matrix]
    n = len(a)
    sign = 1
    prev = 1
    for k in range(n - 1):
        pivot_row = k
        while pivot_row < n and a[pivot_row][k] == 0:
            pivot_row += 1
        if pivot_row == n:
            return 0
        if pivot_row != k:
            a[k], a[pivot_row] = a[pivot_row], a[k]
            sign *= -1
        pivot = a[k][k]
        for i in range(k + 1, n):
            for j in range(k + 1, n):
                a[i][j] = (a[i][j] * pivot - a[i][k] * a[k][j]) // prev
            a[i][k] = 0
        prev = pivot
    return sign * a[n - 1][n - 1]


def is_prime(x):
    if x < 2:
        return False
    for q in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if x % q == 0:
            return x == q
        d = x - 1
        r = 0
        while d % 2 == 0:
            d //= 2
            r += 1
        v = pow(q, d, x)
        if v in (1, x - 1):
            continue
        for _ in range(r - 1):
            v = v * v % x
            if v == x - 1:
                break
        else:
            return False
    return True


def random_prime(rng, lo, hi):
    while True:
        x = rng.randint(lo, hi)
        if is_prime(x):
            return x


def emit(n, p, mat):
    print(n, p)
    for row in mat:
        print(*row)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(2, 3, [[1, 0], [0, 1]])
elif seed == 2:
    n, p = 50, 999999937
    while True:
        mat = [[rng.randint(0, p - 1) for _ in range(n)] for _ in range(n)]
        if bareiss_det(mat) != 0:
            break
    emit(n, p, mat)
elif seed == 3:
    n, p = 50, 999999733
    while True:
        perm = list(range(n))
        rng.shuffle(perm)
        mat = [[0] * n for _ in range(n)]
        for i in range(n):
            mat[i][perm[i]] = rng.randint(1, p - 1)
            for _ in range(3):
                mat[i][rng.randrange(n)] = rng.randint(0, p - 1)
        if bareiss_det(mat) != 0:
            break
    emit(n, p, mat)
elif seed == 4:
    n, p = 50, 3
    while True:
        mat = [[rng.randint(0, p - 1) for _ in range(n)] for _ in range(n)]
        d = bareiss_det(mat)
        if d != 0 and d % p == 0:
            break
    emit(n, p, mat)
else:
    n = rng.randint(2, 40)
    p = random_prime(rng, 3, 10**9)
    while True:
        mat = [[rng.randint(0, p - 1) for _ in range(n)] for _ in range(n)]
        if bareiss_det(mat) != 0:
            break
    emit(n, p, mat)
