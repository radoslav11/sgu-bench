"""Tests for p283 (Mechanics): two circles, elastic hit, integer data
(|coords|, |speeds|, radii <= 10^4; masses <= 100; 0 <= t <= 100).

Every candidate is validated with a float simulation: separated at the
start and at time t, and any collision is non-grazing and away from the
ends of the interval. Seeds 2-3 use extreme coordinates and speeds (with
and without a hit); other seeds mix head-on hits, t=0, resting circle,
equal masses.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def simulate(ax, ay, avx, avy, ra, ma, bx, by, bvx, bvy, rb, mb, t):
    """Return (ok_margins, hit_time or None)."""
    R = ra + rb
    px, py = ax - bx, ay - by
    vx, vy = avx - bvx, avy - bvy
    d0 = math.hypot(px, py)
    if d0 <= R + 0.5:
        return False, None
    a = vx * vx + vy * vy
    b = 2 * (px * vx + py * vy)
    c = px * px + py * py - R * R
    hit = None
    if a > 1e-12:
        disc = b * b - 4 * a * c
        if disc >= 0:
            s = (-b - math.sqrt(disc)) / (2 * a)
            if 0 < s <= t:
                hit = s
                # non-grazing: decent normal closing speed at impact
                nx = (px + vx * s) / R
                ny = (py + vy * s) / R
                rel = vx * nx + vy * ny
                if abs(rel) < 0.5:
                    return False, None
                if s < 0.05 or s > t - 0.05:
                    return False, None
    # final separation check
    if hit is None:
        fx, fy = px + vx * t, py + vy * t
        if math.hypot(fx, fy) <= R + 0.5:
            return False, None
    else:
        nx = (px + vx * hit) / R
        ny = (py + vy * hit) / R
        rel = vx * nx + vy * ny
        k = 2.0 * rel
        vx2 = vx - k * nx
        vy2 = vy - k * ny
        # relative velocity after elastic hit reverses normal component
        # (exact masses applied below per circle; relative motion is the
        # same regardless of mass split for the separation check)
        ka = 2.0 * mb / (ma + mb) * rel
        kb = 2.0 * ma / (ma + mb) * rel
        avx2, avy2 = avx - ka * nx, avy - ka * ny
        bvx2, bvy2 = bvx + kb * nx, bvy + kb * ny
        rest = t - hit
        fax = ax + avx * hit + avx2 * rest
        fay = ay + avy * hit + avy2 * rest
        fbx = bx + bvx * hit + bvx2 * rest
        fby = by + bvy * hit + bvy2 * rest
        if math.hypot(fax - fbx, fay - fby) <= R + 0.5:
            return False, None
    return True, hit


def emit(case):
    ax, ay, avx, avy, ra, ma, bx, by, bvx, bvy, rb, mb, t = case
    print(ax, ay, avx, avy, ra, ma)
    print(bx, by, bvx, bvy, rb, mb)
    print(t)


def random_case(big, want_hit, t_choice=None):
    while True:
        C = 10000 if big else 100
        V = 10000 if big else 50
        ra = rng.randint(1, 10000 if big else 10)
        rb = rng.randint(1, 10000 if big else 10)
        ma = rng.randint(1, 100)
        mb = rng.randint(1, 100)
        t = t_choice if t_choice is not None else rng.randint(0, 100)
        ax, ay = rng.randint(-C, C), rng.randint(-C, C)
        bx, by = rng.randint(-C, C), rng.randint(-C, C)
        if want_hit:
            # aim A roughly at B with a small offset, B slower/random
            d = max(1.0, math.hypot(bx - ax, by - ay))
            speed = rng.randint(max(1, int(d / max(1, t or 1))), V)
            avx = int(round((bx - ax) / d * speed)) + rng.randint(-2, 2)
            avy = int(round((by - ay) / d * speed)) + rng.randint(-2, 2)
            bvx, bvy = rng.randint(-5, 5), rng.randint(-5, 5)
        else:
            avx, avy = rng.randint(-V, V), rng.randint(-V, V)
            bvx, bvy = rng.randint(-V, V), rng.randint(-V, V)
        case = (ax, ay, avx, avy, ra, ma, bx, by, bvx, bvy, rb, mb, t)
        ok, hit = simulate(*[float(x) for x in case[:4]], case[4], case[5],
                           *[float(x) for x in case[6:10]], case[10],
                           case[11], float(case[12]))
        if not ok:
            continue
        if want_hit and hit is None:
            continue
        if not want_hit and hit is not None:
            continue
        return case


if seed == 1:
    emit((0, 0, 1, 0, 1, 1, 10, 0, -1, 0, 1, 1, 5))
elif seed == 2:
    # extreme values with a guaranteed hit
    emit(random_case(True, True, 100))
elif seed == 3:
    # extreme values, no hit
    emit(random_case(True, False, 100))
elif seed == 4:
    # t = 0
    emit((-10000, -10000, 10000, 10000, 10000, 100, 10000, 10000, -10000,
          -10000, 1, 1, 0))
elif seed == 5:
    # both circles at rest
    emit((-5000, 3, 0, 0, 17, 42, 5000, -3, 0, 0, 23, 7, 100))
elif seed == 6:
    # B at rest, head-on equal masses
    emit((0, 0, 7, 0, 5, 50, 100, 0, 0, 0, 5, 50, 30))
elif seed in (7, 8):
    emit(random_case(False, True))
else:
    emit(random_case(False, seed % 2 == 0))
