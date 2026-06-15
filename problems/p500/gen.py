"""Tests for p500 (Circular Island): min/max area of Java's land.

A circle of radius r centred at the origin holds n Java and m See++
villages, all integer points strictly inside the disk and at least r/10
from the boundary, with a straight line separating the two tribes.

To guarantee separability we pick a random direction u and a signed gap
g, place every village inside the disk of radius 0.85r, then keep only
points whose projection onto u is >= g (Java) or <= -g (See++). This
leaves a clear corridor of width 2g between the tribes.

Seed 1 is the minimal case (one village each). Seeds 2 and 3 are
maximal (n = m = 100000) with r at the limit; seed 2 spreads points
broadly, seed 3 packs both tribes into thin slabs hugging the corridor
so the hulls have many vertices. Later seeds randomise sizes, radius and
the separating direction.
"""

import math
import random
import sys

N_MAX = 100000
R_MAX = 10**9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(r, ja, se):
    out = [str(r), str(len(ja))]
    out += [f"{x} {y}" for x, y in ja]
    out.append(str(len(se)))
    out += [f"{x} {y}" for x, y in se]
    sys.stdout.write("\n".join(out))
    sys.stdout.write("\n")


def gen_points(r, n_target, m_target, thin=False):
    # random separating direction u, corridor half-width gap
    ang = rng.uniform(0, 2 * math.pi)
    ux, uy = math.cos(ang), math.sin(ang)
    rlim = 0.85 * r
    gap = max(2.0, 0.03 * r)

    ja = set()
    se = set()
    # bound on how many attempts: oversample
    target = n_target + m_target
    attempts = 0
    max_attempts = 40 * target + 1000
    while (len(ja) < n_target or len(se) < m_target) and attempts < max_attempts:
        attempts += 1
        # sample a point inside disk of radius rlim
        rad = rlim * math.sqrt(rng.random())
        a = rng.uniform(0, 2 * math.pi)
        x = int(round(rad * math.cos(a)))
        y = int(round(rad * math.sin(a)))
        if x * x + y * y > rlim * rlim:
            continue
        proj = x * ux + y * uy
        if thin:
            # keep points within a thin slab close to the corridor edge
            if gap <= proj <= gap + max(2.0, 0.05 * r) and len(ja) < n_target:
                ja.add((x, y))
            elif -gap - max(2.0, 0.05 * r) <= proj <= -gap and len(se) < m_target:
                se.add((x, y))
        else:
            if proj >= gap and len(ja) < n_target:
                ja.add((x, y))
            elif proj <= -gap and len(se) < m_target:
                se.add((x, y))

    # ensure at least one on each side
    if not ja:
        ja.add((int(round(gap * ux + 1)), int(round(gap * uy))))
    if not se:
        se.add((int(round(-gap * ux - 1)), int(round(-gap * uy))))
    return list(ja), list(se)


if seed == 1:
    emit(10, [(3, 0)], [(-3, 0)])
    sys.exit()

if seed == 2:
    r = R_MAX
    ja, se = gen_points(r, N_MAX, N_MAX, thin=False)
    emit(r, ja, se)
    sys.exit()

if seed == 3:
    r = R_MAX
    ja, se = gen_points(r, N_MAX, N_MAX, thin=True)
    emit(r, ja, se)
    sys.exit()

# randomized mid-size
r = rng.randint(100, R_MAX)
n = rng.randint(1, 3000)
m = rng.randint(1, 3000)
ja, se = gen_points(r, n, m, thin=(seed % 2 == 0))
emit(r, ja, se)
