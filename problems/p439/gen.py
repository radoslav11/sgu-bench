"""Tests for p439 (A Secret Book): min rotation of B, then locate it in A.

The statement's numeric bounds are lost; tests use 1 <= M < N <= 200000
(matching the O(N log N) reference). Each test plants the minimal
rotation of B (with 0 or 1 mutated characters) into a rotation of A, so
a solution always exists. Seed 1 is minimal, seeds 2-3 maximal: seed 2
a binary-alphabet near-periodic pair with M = N-1, seed 3 uniform
letters with one planted mismatch and the match hidden near shift N/2.
Seed 4 is a single repeated letter, seed 5 a short-period B over {A,B}.
"""

import random
import sys


def least_rotation_start(s):
    s2 = s + s
    f = [-1] * len(s2)
    k = 0
    for j in range(1, len(s2)):
        sj = s2[j]
        i = f[j - k - 1]
        while i != -1 and sj != s2[k + i + 1]:
            if sj < s2[k + i + 1]:
                k = j - i - 1
            i = f[i]
        if sj != s2[k + i + 1]:
            if sj < s2[k]:
                k = j
            f[j - k] = -1
        else:
            f[j - k] = i + 1
    return k


def rand_string(rng, length, alphabet):
    return "".join(rng.choice(alphabet) for _ in range(length))


def build(rng, n, m, alphabet, mismatches, rot_a, rot_b):
    b_final = rand_string(rng, m, alphabet)
    start = least_rotation_start(b_final)
    pattern = list((b_final + b_final)[start:start + m])

    if mismatches:
        pos = rng.randrange(m)
        pattern[pos] = rng.choice(alphabet)

    a_final = "".join(pattern) + rand_string(rng, n - m, alphabet)
    a_in = a_final[-rot_a:] + a_final[:-rot_a] if rot_a else a_final
    b_in = b_final[-rot_b:] + b_final[:-rot_b] if rot_b else b_final

    print(n, m)
    print(a_in)
    print(b_in)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2, 1)
    print("BA")
    print("A")
elif seed == 2:
    n, m = 200000, 199999
    build(rng, n, m, "AB", 0, n // 2, rng.randrange(m))
elif seed == 3:
    n, m = 200000, 100000
    build(rng, n, m, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, n // 2,
          rng.randrange(m))
elif seed == 4:
    print(1000, 999)
    print("A" * 1000)
    print("A" * 999)
elif seed == 5:
    n, m = 100000, 60000
    b_final = "AB" * (m // 2)
    pattern = list(b_final[least_rotation_start(b_final):]
                   + b_final[:least_rotation_start(b_final)])
    a_final = "".join(pattern) + rand_string(rng, n - m, "AB")
    rot = n // 3
    print(n, m)
    print(a_final[-rot:] + a_final[:-rot])
    print(b_final)
else:
    n = rng.randint(2, 5000)
    m = rng.randint(1, n - 1)
    alphabet = rng.choice(["AB", "ABC", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"])
    build(rng, n, m, alphabet, rng.randint(0, 1), rng.randrange(n),
          rng.randrange(m))
