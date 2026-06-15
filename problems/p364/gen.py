"""Tests for p364 (Lemmings). N <= 100, s <= 10, M <= 100 horizontal
platforms with integer coords in 0..10000 that pairwise do not touch.
Guarantees enforced here: point A touches no platform, and any fall (from
A or off either platform end) lands strictly inside some platform or
falls forever. Home (a, b) always lies on a platform.

Platforms are sampled with distinct heights, then the whole configuration
is validated (every drop point must land on an inner point); invalid
configurations are resampled from the same rng, so output stays
deterministic per seed. The cascade builder chains platforms downward so
the platform under A leads through many levels, which forces reflector
turns and exercises the (turns, distance) optimization.

Seed plan:
- seed 1: minimal: N=1, s=1, one platform, home straight ahead.
- seed 2: maximal: N=100, s=10, M=100 cascade, home at the bottom.
- seed 3: maximal: N=100, s=10, M=100 scattered random platforms.
- seed 4: home behind the start direction with N=1 (needs a turn that
  sacrifices the only lemming -> 0 0).
- seed 5: home platform exists but A falls past everything (0 0).
- seed 6: home exactly at a platform endpoint.
- seeds 7+: random mid-size configurations.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10000


def landing(x, h, plats):
    best = None
    for idx, (l, r, hh) in enumerate(plats):
        if hh < h and l <= x <= r:
            if best is None or hh > plats[best][2]:
                best = idx
    return best


def valid(plats, ax, ah):
    heights = {}
    for l, r, h in plats:
        if not (0 <= l < r <= LIM and 0 <= h <= LIM):
            return False
        for ol, orr in heights.get(h, []):
            if not (r < ol or orr < l):
                return False
        heights.setdefault(h, []).append((l, r))

    def drop_ok(x, h):
        j = landing(x, h, plats)
        if j is None:
            return True
        l, r, _ = plats[j]
        return l < x < r

    for l, r, h in plats:
        if h == ah and l <= ax <= r:
            return False
        if not drop_ok(l, h) or not drop_ok(r, h):
            return False
    if not (0 <= ax <= LIM and 0 <= ah <= LIM):
        return False
    return drop_ok(ax, ah)


def emit(n, s, ax, ah, a, b, plats):
    print(n, s)
    print(ax, ah, a, b)
    print(len(plats))
    for l, r, h in plats:
        print(l, r, h)


def random_plats(rng, m, want_under_a=True):
    while True:
        heights = rng.sample(range(0, LIM), m + 1)
        ah = max(heights)
        plat_h = [h for h in heights if h != ah]
        plats = []
        for h in plat_h:
            l = rng.randint(0, LIM - 2)
            r = rng.randint(l + 1, min(LIM, l + rng.choice([5, 50, 2000])))
            plats.append((l, r, h))
        if want_under_a:
            l, r, _ = plats[0]
            ax = rng.randint(l + 1, r - 1) if r - l >= 2 else l
        else:
            ax = rng.randint(0, LIM)
        if valid(plats, ax, ah):
            return plats, ax, ah


def cascade_plats(rng, m):
    # Chain platforms downward: each platform contains the previous fall
    # point strictly inside, and the next fall happens from one of its ends.
    while True:
        ax = rng.randint(3000, 7000)
        ah = LIM
        plats = []
        x = ax
        h = ah
        ok = True
        for _ in range(m):
            h -= rng.randint(2, (h - 1) // max(1, (m * 2)) + 2)
            if h <= 0:
                ok = False
                break
            wl = rng.randint(1, 40)
            wr = rng.randint(1, 40)
            l, r = x - wl, x + wr
            if l < 0 or r > LIM:
                ok = False
                break
            plats.append((l, r, h))
            x = l if rng.random() < 0.5 else r
        if ok and valid(plats, ax, ah):
            return plats, ax, ah


if seed == 1:
    emit(1, 1, 5, 10, 8, 5, [(0, 9, 5)])
elif seed == 2:
    plats, ax, ah = cascade_plats(rng, 100)
    bottom = min(plats, key=lambda p: p[2])
    l, r, h = bottom
    a = rng.randint(l, r)
    emit(100, 10, ax, ah, a, h, plats)
elif seed == 3:
    plats, ax, ah = random_plats(rng, 100)
    pl, pr, ph = rng.choice(plats)
    a = rng.randint(pl, pr)
    emit(100, 10, ax, ah, a, ph, plats)
elif seed == 4:
    # Home is behind the lemming: the only lemming must be sacrificed.
    emit(1, 1, 50, 10, 10, 5, [(5, 100, 5)])
elif seed == 5:
    # A's column misses every platform: everyone falls forever.
    emit(50, 3, 9000, 9999, 50, 100, [(10, 90, 100), (200, 300, 50)])
elif seed == 6:
    emit(10, 2, 50, 10, 100, 5, [(5, 100, 5)])
else:
    n = rng.randint(1, 100)
    s = rng.randint(1, 10)
    m = rng.randint(1, 100)
    if rng.random() < 0.5:
        plats, ax, ah = cascade_plats(rng, m)
    else:
        plats, ax, ah = random_plats(rng, m)
    pl, pr, ph = rng.choice(plats)
    a = rng.randint(pl, pr)
    emit(n, s, ax, ah, a, ph, plats)
