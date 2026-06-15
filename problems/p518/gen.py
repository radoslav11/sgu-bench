"""Tests for p518 (Kidnapping): n <= 200 intersections, route of k <= 200 roads.

The graph is a symmetric n x n matrix of road lengths (0 = no road,
1..200 = a road of that length), with zero diagonal. The route is k values
in 1..200. Seed 1 is the sample. Seeds 2 and 3 are maximal (n = k = 200):
seed 2 is a dense graph where many length values repeat (huge BFS state
branching), seed 3 fixes every road to the same length so the route can be
followed almost everywhere (maximal reachable-state set).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(mat, route):
    n = len(mat)
    out = [str(n)]
    for row in mat:
        out.append(" ".join(map(str, row)))
    out.append(str(len(route)))
    out.append(" ".join(map(str, route)))
    print("\n".join(out))


def random_symmetric(n, density, val_lo, val_hi):
    mat = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if rng.random() < density:
                w = rng.randint(val_lo, val_hi)
                mat[i][j] = w
                mat[j][i] = w
    return mat


if seed == 1:
    emit(
        [[0, 1, 2, 0], [1, 0, 1, 0], [2, 1, 0, 2], [0, 0, 2, 0]],
        [1, 1, 2],
    )
elif seed == 2:
    # Maximal dense graph using only a few distinct lengths so the route
    # branches heavily; route alternates those lengths.
    n = 200
    mat = random_symmetric(n, 0.9, 1, 5)
    route = [rng.randint(1, 5) for _ in range(200)]
    emit(mat, route)
elif seed == 3:
    # Every existing road has length 7; route is all 7s -> the walk can be
    # extended to (almost) every node at every step.
    n = 200
    mat = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            mat[i][j] = mat[j][i] = 7
    route = [7] * 200
    emit(mat, route)
elif seed == 4:
    # Disconnected / sparse graph, short impossible route -> answer 0.
    n = 50
    mat = random_symmetric(n, 0.05, 50, 200)
    route = [rng.randint(1, 200) for _ in range(200)]
    emit(mat, route)
elif seed == 5:
    # Minimal n=2 with a single road; route that must end at node 2.
    emit([[0, 3], [3, 0]], [3, 3, 3])
else:
    n = rng.randint(2, 200)
    density = rng.choice([0.1, 0.3, 0.6, 0.9])
    hi = rng.choice([3, 10, 50, 200])
    mat = random_symmetric(n, density, 1, hi)
    k = rng.randint(1, 200)
    route = [rng.randint(1, hi) for _ in range(k)]
    emit(mat, route)
