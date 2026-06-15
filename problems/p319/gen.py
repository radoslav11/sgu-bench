"""Tests for p319 (Kalevich Strikes Back): N <= 60000 frames on a W x H
canvas (W, H <= 1e8), frames pairwise without common points (so nested
or disjoint), coordinates strictly inside the canvas.

Seed 1 is the minimal canvas with one frame, seed 2 a 60000-deep nesting
chain, seed 3 a 60000-frame disjoint grid, later seeds random nesting
forests built by recursive region splitting, with corners emitted in
random orientation.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

W = H = 10**8


def emit(rects):
    print(len(rects))
    print(W, H)
    out = []
    for x1, y1, x2, y2 in rects:
        if rng.random() < 0.5:
            x1, x2 = x2, x1
        if rng.random() < 0.5:
            y1, y2 = y2, y1
        out.append(f"{x1} {y1} {x2} {y2}")
    print("\n".join(out))


if seed == 1:
    W = H = 3
    emit([(1, 1, 2, 2)])
    sys.exit()

if seed == 2:
    rects = [(k, k, W - k, H - k) for k in range(1, 60001)]
    rng.shuffle(rects)
    emit(rects)
    sys.exit()

if seed == 3:
    step = (W - 2) // 245
    rects = []
    for r in range(245):
        for c in range(245):
            if len(rects) == 60000:
                break
            bx, by = c * step + 1, r * step + 1
            rects.append((bx, by,
                          bx + rng.randint(1, step - 2),
                          by + rng.randint(1, step - 2)))
    rng.shuffle(rects)
    emit(rects)
    sys.exit()

if seed % 2 == 0:
    W = rng.randint(3, 100)
    H = rng.randint(3, 100)
    target = rng.randint(1, 30)
else:
    target = rng.randint(2000, 20000)

# regions are boxes whose interiors (with a margin of 1 from every edge)
# may hold future frames, so frames never touch each other or the canvas
regions = [(0, 0, W, H)]
rects = []
nest_prob = 0.55 if seed % 3 else 0.85

while regions and len(rects) < target:
    idx = rng.randrange(len(regions))
    x1, y1, x2, y2 = regions.pop(idx)
    if x2 - x1 < 3 or y2 - y1 < 3:
        continue
    if rng.random() > nest_prob and x2 - x1 >= 6 and y2 - y1 >= 6:
        if rng.random() < 0.5:
            mid = rng.randint(x1 + 3, x2 - 3)
            regions += [(x1, y1, mid, y2), (mid, y1, x2, y2)]
        else:
            mid = rng.randint(y1 + 3, y2 - 3)
            regions += [(x1, y1, x2, mid), (x1, mid, x2, y2)]
        continue
    rx1 = rng.randint(x1 + 1, x2 - 2)
    rx2 = rng.randint(rx1 + 1, x2 - 1)
    ry1 = rng.randint(y1 + 1, y2 - 2)
    ry2 = rng.randint(ry1 + 1, y2 - 1)
    rects.append((rx1, ry1, rx2, ry2))
    regions += [(rx1, ry1, rx2, ry2),
                (x1, y1, rx1, y2), (rx2, y1, x2, y2),
                (rx1, y1, rx2, ry1), (rx1, ry2, rx2, y2)]

if not rects:
    rects = [(1, 1, 2, 2)] if W >= 3 and H >= 3 else []
if not rects:
    W = H = 3
    rects = [(1, 1, 2, 2)]
rng.shuffle(rects)
emit(rects)
