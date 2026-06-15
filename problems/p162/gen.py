"""Tests for p162 (Pyramids): tetrahedron volume from its 6 edge lengths.

Input is six positive integers <= 1000 in the order AB AC AD BC BD CD,
guaranteed to form a (non-degenerate) tetrahedron. Validity is verified
exactly: every face must satisfy the strict triangle inequality and the
Cayley-Menger determinant (288 V^2, an integer here) must be positive.

Seed 1 is the minimal regular tetrahedron, seed 2 the maximal one (all
edges 1000), seed 3 the extreme sliver from sample 2. Later seeds are
random valid tetrahedra over various ranges, including near-degenerate
(almost flat) ones found by minimizing the determinant.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def cm_det(e):
    # 5x5 Cayley-Menger determinant over the integers (Bareiss algorithm);
    # equals 288 * V^2 for edge order AB AC AD BC BD CD.
    ab, ac, ad, bc, bd, cd = (x * x for x in e)
    m = [
        [0, 1, 1, 1, 1],
        [1, 0, ab, ac, ad],
        [1, ab, 0, bc, bd],
        [1, ac, bc, 0, cd],
        [1, ad, bd, cd, 0],
    ]
    sign = 1
    prev = 1
    for i in range(4):
        if m[i][i] == 0:
            swap = next((r for r in range(i + 1, 5) if m[r][i] != 0), -1)
            if swap == -1:
                return 0
            m[i], m[swap] = m[swap], m[i]
            sign = -sign
        for r in range(i + 1, 5):
            for c in range(i + 1, 5):
                m[r][c] = (m[r][c] * m[i][i] - m[r][i] * m[i][c]) // prev
        prev = m[i][i]
    return sign * m[4][4]


def valid(e):
    ab, ac, ad, bc, bd, cd = e
    faces = [(ab, ac, bc), (ab, ad, bd), (ac, ad, cd), (bc, bd, cd)]
    for x, y, z in faces:
        if x + y <= z or x + z <= y or y + z <= x:
            return False
    return cm_det(e) > 0


def random_tetra(rng, lo, hi):
    while True:
        e = [rng.randint(lo, hi) for _ in range(6)]
        if valid(e):
            return e


if seed == 1:
    print(1, 1, 1, 1, 1, 1)
elif seed == 2:
    print(1000, 1000, 1000, 1000, 1000, 1000)
elif seed == 3:
    print(1000, 1000, 1000, 3, 4, 5)
elif seed == 4:
    # nearly flat tetrahedron: greedily perturb edges to shrink the
    # determinant while keeping it strictly positive
    e = random_tetra(rng, 200, 900)
    for _ in range(3000):
        i = rng.randrange(6)
        delta = rng.choice([-1, 1])
        cand = e[:]
        cand[i] += delta
        if all(1 <= x <= 1000 for x in cand) and valid(cand):
            if cm_det(cand) < cm_det(e):
                e = cand
    print(*e)
elif seed == 5:
    # tiny edges
    print(*random_tetra(rng, 1, 5))
elif seed == 6:
    # all edges near the upper limit
    print(*random_tetra(rng, 990, 1000))
else:
    print(*random_tetra(rng, 1, 1000))
