"""Tests for p260 (Puzzle): N <= 200 cells, cell i toggles its predefined
set when clicked, initial coloring is a line of N zeros/ones.

Seed 1 is minimal (N=1, already monochrome). Seed 2 is maximal with
dense random groups (~N/2 cells each, worst case for Gaussian
elimination). Seed 3 is maximal with a 10x20 grid puzzle structure
(cell + 4 neighbours). Seed 4 forces -1 via two untouchable cells of
different colors. Seed 5 has empty groups mixed in. The rest are
random sparse/dense mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(groups, colors):
    n = len(groups)
    out = [str(n)]
    for g in groups:
        out.append(" ".join(map(str, [len(g)] + sorted(g))))
    out.append(" ".join(map(str, colors)))
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit([[1]], [0])
elif seed == 2:
    n = 200
    groups = [rng.sample(range(1, n + 1), rng.randint(n // 3, 2 * n // 3))
              for _ in range(n)]
    emit(groups, [rng.randint(0, 1) for _ in range(n)])
elif seed == 3:
    rows, cols = 10, 20
    n = rows * cols

    def cell(r, c):
        return r * cols + c + 1

    groups = []
    for r in range(rows):
        for c in range(cols):
            g = [cell(r, c)]
            for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nr, nc = r + dr, c + dc
                if 0 <= nr < rows and 0 <= nc < cols:
                    g.append(cell(nr, nc))
            groups.append(g)
    emit(groups, [rng.randint(0, 1) for _ in range(n)])
elif seed == 4:
    n = 200
    groups = []
    for i in range(1, n + 1):
        if i <= 2:
            groups.append([])
        else:
            pool = list(range(3, n + 1))
            groups.append(rng.sample(pool, rng.randint(1, 10)))
    colors = [rng.randint(0, 1) for _ in range(n)]
    colors[0], colors[1] = 0, 1
    emit(groups, colors)
elif seed == 5:
    n = 200
    groups = []
    for i in range(1, n + 1):
        k = rng.randint(0, 5)
        groups.append(rng.sample(range(1, n + 1), k))
    emit(groups, [rng.randint(0, 1) for _ in range(n)])
else:
    n = rng.randint(2, 200)
    dense = rng.random() < 0.5
    groups = []
    for i in range(1, n + 1):
        if dense:
            k = rng.randint(1, n)
        else:
            k = rng.randint(0, min(6, n))
        g = rng.sample(range(1, n + 1), k)
        if rng.random() < 0.7 and i not in g:
            g = g[:-1] + [i] if g else [i]
        groups.append(list(set(g)))
    emit(groups, [rng.randint(0, 1) for _ in range(n)])
