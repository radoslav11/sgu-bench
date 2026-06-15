"""Tests for p545 (Cut the rope...).

Input: line 1 "n m" (2<=n<=20, 1<=m<=1000); line 2 n attachment x's
(1<=xi<=200); line 3 n rope lengths (1<=li<=200); line 4 the n-1 distinct
cut indices (1-based); line 5 m distinct increasing query times in [0,1000]
given with <=3 decimals.

The configuration must be physically realisable: the lower ends of all
ropes can be tied to a single ball, i.e. the intersection of the disks
(center (xi,0), radius li) is non-empty. We guarantee this by picking a
ball anchor point B with by<0 and giving each rope a length >= dist(xi,B)
so B lies in every disk. We also forbid two ropes sharing both the same
attachment point and the same length.

Seed 1 is the minimal n=2 case. Seeds 2-3 are maximal n=20, m=1000 with
two different attachment/length layouts and dense time sampling. Later
seeds vary n, m and geometry.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def build_config(n, rng):
    # Pick a ball anchor that all ropes can reach with integer lengths
    # bounded by 200, then assign distinct (x, l) pairs.
    while True:
        bx = rng.uniform(20, 180)
        by = -rng.uniform(5, 120)
        xs = []
        ls = []
        used = set()
        ok = True
        for _ in range(n):
            for _try in range(200):
                x = rng.randint(1, 200)
                dist = math.hypot(x - bx, by)
                lo = int(math.ceil(dist)) + 1
                if lo > 200:
                    continue
                l = rng.randint(lo, 200)
                if (x, l) in used:
                    continue
                used.add((x, l))
                xs.append(x)
                ls.append(l)
                break
            else:
                ok = False
                break
        if ok and len(xs) == n:
            return xs, ls


def gen_times(m, rng):
    # m distinct increasing times in [0,1000] with 3 decimals.
    vals = set()
    while len(vals) < m:
        vals.add(round(rng.uniform(0, 1000), 3))
    return sorted(vals)


def emit(n, m, xs, ls, rng):
    cut = list(range(1, n + 1))
    rng.shuffle(cut)
    cut = cut[: n - 1]
    times = gen_times(m, rng)
    print(n, m)
    print(*xs)
    print(*ls)
    print(*cut)
    print(*("{:.3f}".format(t) for t in times))


if seed == 1:
    # minimal: two ropes, single query at t=0
    xs, ls = build_config(2, random.Random(1))
    emit(2, 1, xs, ls, rng)
    sys.exit()

if seed == 2:
    n, m = 20, 1000
    xs, ls = build_config(n, rng)
    emit(n, m, xs, ls, rng)
    sys.exit()

if seed == 3:
    # max with attachments clustered tightly so many circles interact
    n, m = 20, 1000
    while True:
        bx = rng.uniform(90, 110)
        by = -rng.uniform(60, 120)
        xs = []
        ls = []
        used = set()
        ok = True
        base_x = rng.randint(80, 120)
        for _ in range(n):
            for _try in range(200):
                x = base_x + rng.randint(-15, 15)
                if not (1 <= x <= 200):
                    continue
                dist = math.hypot(x - bx, by)
                lo = int(math.ceil(dist)) + 1
                if lo > 200:
                    continue
                l = rng.randint(lo, 200)
                if (x, l) in used:
                    continue
                used.add((x, l))
                xs.append(x)
                ls.append(l)
                break
            else:
                ok = False
                break
        if ok and len(xs) == n:
            break
    emit(n, m, xs, ls, rng)
    sys.exit()

n = rng.randint(2, 20)
m = rng.randint(1, 1000)
xs, ls = build_config(n, rng)
emit(n, m, xs, ls, rng)
