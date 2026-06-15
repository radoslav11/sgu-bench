"""Tests for p501 (Octahedron And Dominoes): count triangular-domino tilings.

n is 1..4. The octahedron is given as 2n layers; layer i (0-indexed) has
8*min(i, 2n-1-i)+4 small triangles, each '.' (white, to be covered) or
'*' (black, left uncovered). The answer counts perfect coverings of the
white cells by bent triangular dominoes.

Seed 1 is the minimal case (n=1). Seeds 2 and 3 are the maximal n=4
octahedron: seed 2 is all-white (the heaviest DP, largest tiling count),
seed 3 is a dense random board. Later seeds vary n and the black-cell
density, including all-white and mostly-black extremes.
"""

import random
import sys


def layer_len(i, n):
    return 8 * min(i, 2 * n - 1 - i) + 4


def emit(n, prob_black, rng):
    out = [str(n)]
    for i in range(2 * n):
        L = layer_len(i, n)
        out.append("".join("*" if rng.random() < prob_black else "." for _ in range(L)))
    sys.stdout.write("\n".join(out))
    sys.stdout.write("\n")


def emit_fixed(n, ch, rng):
    out = [str(n)]
    for i in range(2 * n):
        L = layer_len(i, n)
        out.append(ch * L)
    sys.stdout.write("\n".join(out))
    sys.stdout.write("\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # minimal: n=1, two layers of 4 cells; all white
    emit_fixed(1, ".", rng)
    sys.exit()

if seed == 2:
    emit_fixed(4, ".", rng)
    sys.exit()

if seed == 3:
    emit(4, 0.4, rng)
    sys.exit()

if seed == 4:
    emit_fixed(4, "*", rng)
    sys.exit()

n = rng.randint(1, 4)
prob = rng.choice([0.0, 0.15, 0.3, 0.5, 0.7])
emit(n, prob, rng)
