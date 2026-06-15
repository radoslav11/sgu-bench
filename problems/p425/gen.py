"""Tests for p425 (Control function): 1 <= n, m <= 1000, non-negative
matrix entries (kept <= 10^9), every row below the first differs from
the first row in at least one column.

Seed 1 is minimal, seed 2 a maximal single-mismatch-per-row test (the
hard coloring case), seed 3 a maximal fully random matrix; later seeds
include a planted 45-clique constraint graph (forces 45 colors), tiny
value pools and single-row/column shapes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(matrix):
    print(len(matrix), len(matrix[0]))
    out = []
    for row in matrix:
        out.append(" ".join(map(str, row)))
    print("\n".join(out))


def single_mismatch(n, m, pool):
    # Each row equals the first except in one random column.
    first = [rng.choice(pool) for _ in range(m)]
    rows = [first]
    for _ in range(n - 1):
        row = first[:]
        j = rng.randrange(m)
        v = rng.choice(pool)
        while v == first[j]:
            v = rng.choice(pool)
        row[j] = v
        rows.append(row)
    return rows


if seed == 1:
    emit([[0]])
elif seed == 2:
    emit(single_mismatch(1000, 1000, list(range(2000))))
elif seed == 3:
    n, m = 1000, 1000
    first = [rng.randint(0, 10**9) for _ in range(m)]
    rows = [first]
    for _ in range(n - 1):
        row = [rng.randint(0, 10**9) for _ in range(m)]
        if row == first:
            row[0] = (row[0] + 1) % (10**9 + 1)
        rows.append(row)
    emit(rows)
elif seed == 4:
    # First row holds 1000 distinct values; the other rows plant a clique
    # on 45 of those values in the constraint graph (needs 45 colors).
    m = 1000
    first = list(range(m))
    rows = [first]
    clique = rng.sample(range(m), 45)
    pairs = [(u, v) for i, u in enumerate(clique) for v in clique[i + 1:]]
    rng.shuffle(pairs)
    for u, v in pairs[:999]:
        row = first[:]
        row[first.index(u)] = v
        rows.append(row)
    emit(rows)
elif seed == 5:
    emit(single_mismatch(1000, 1000, [0, 1, 2, 3]))
elif seed == 6:
    n = 1000
    first = [rng.randint(0, 5)]
    rows = [first]
    for _ in range(n - 1):
        v = rng.randint(0, 5)
        while v == first[0]:
            v = rng.randint(0, 5)
        rows.append([v])
    emit(rows)
elif seed == 7:
    emit([[rng.randint(0, 10**9) for _ in range(1000)]])
elif seed == 8:
    emit(single_mismatch(50, 4, list(range(60))))
else:
    n, m = rng.randint(2, 300), rng.randint(1, 300)
    emit(single_mismatch(n, m, list(range(rng.randint(2, 500)))))
