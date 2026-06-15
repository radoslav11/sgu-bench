"""Tests for p265 (Wizards): N <= 1000 spells (T/S/R), M <= 100000 points.

All numbers real, |value| <= 1000; scaling factors |s| <= 1; rotation axis
nonzero. Seed 1 minimal, seeds 2-3 maximal (mixed spells / rotation-heavy),
later seeds random with degenerate axes, zero scalings and zero angles.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def real(lo=-1000.0, hi=1000.0):
    return round(rng.uniform(lo, hi), 3)


def fmt(v):
    return f"{v:.3f}"


def rand_axis():
    while True:
        x, y, z = real(), real(), real()
        if x * x + y * y + z * z > 1e-6:
            return x, y, z


def spell_line(kind):
    if kind == 'T':
        return f"T {fmt(real())} {fmt(real())} {fmt(real())}"
    if kind == 'S':
        return f"S {fmt(real(-1, 1))} {fmt(real(-1, 1))} {fmt(real(-1, 1))}"
    x, y, z = rand_axis()
    return f"R {fmt(x)} {fmt(y)} {fmt(z)} {fmt(real())}"


lines = []

if seed == 1:
    lines.append("1")
    lines.append("T 0.001 -0.001 0.0")
    lines.append("1")
    lines.append("0.0 0.0 0.0")
elif seed == 2:
    n, m = 1000, 100000
    lines.append(str(n))
    for _ in range(n):
        lines.append(spell_line(rng.choice("TSR")))
    lines.append(str(m))
    for _ in range(m):
        lines.append(f"{fmt(real())} {fmt(real())} {fmt(real())}")
elif seed == 3:
    n, m = 1000, 100000
    lines.append(str(n))
    for i in range(n):
        if i % 5 == 0:
            lines.append(f"T {fmt(real())} {fmt(real())} {fmt(real())}")
        else:
            x, y, z = rand_axis()
            angle = rng.choice([90.0, 180.0, 270.0, 360.0, real()])
            lines.append(f"R {fmt(x)} {fmt(y)} {fmt(z)} {fmt(angle)}")
    lines.append(str(m))
    corners = [-1000.0, 1000.0, 0.0]
    for _ in range(m):
        if rng.random() < 0.2:
            lines.append(
                f"{fmt(rng.choice(corners))} {fmt(rng.choice(corners))}"
                f" {fmt(rng.choice(corners))}"
            )
        else:
            lines.append(f"{fmt(real())} {fmt(real())} {fmt(real())}")
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 1000)
    lines.append(str(n))
    for _ in range(n):
        r = rng.random()
        if r < 0.1:
            lines.append("S 0.000 0.000 0.000")
        elif r < 0.2:
            ax = rng.choice([(1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0),
                             (0.0, 0.0, -0.001)])
            angle = rng.choice([0.0, -1000.0, 1000.0, real()])
            lines.append(
                f"R {fmt(ax[0])} {fmt(ax[1])} {fmt(ax[2])} {fmt(angle)}"
            )
        else:
            lines.append(spell_line(rng.choice("TSR")))
    lines.append(str(m))
    for _ in range(m):
        lines.append(f"{fmt(real())} {fmt(real())} {fmt(real())}")

sys.stdout.write("\n".join(lines) + "\n")
