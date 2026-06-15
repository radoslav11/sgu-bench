"""Tests for p470 (The Death Cube): lines of "X Y Z" until EOF.

The statement's constraint parenthesis is lost, so we assume
1 <= X, Y, Z <= 10^6 (the 64-bit-safe range) and up to 10^5 lines.
Seed 1 is the single minimal line, seeds 2-3 are maximal files with
different value distributions, later seeds mix small/parity cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10**6

if seed == 1:
    print(1, 1, 1)
    sys.exit()

lines = []
if seed == 2:
    # Maximal: 10^5 cases, values concentrated at the high end.
    lines.append((LIM, LIM, LIM))
    for _ in range(10**5 - 1):
        lines.append(
            (
                rng.randint(LIM // 2, LIM),
                rng.randint(LIM // 2, LIM),
                rng.randint(LIM // 2, LIM),
            )
        )
elif seed == 3:
    # Maximal count again, but exercising all formula branches: thin
    # dimensions a in {1, 2, 3, 4} with all parities of b and c.
    for _ in range(10**5):
        a = rng.choice([1, 1, 2, 3, 4, rng.randint(1, LIM)])
        b = rng.randint(1, LIM)
        c = rng.randint(1, LIM)
        triple = [a, b, c]
        rng.shuffle(triple)
        lines.append(tuple(triple))
else:
    # Mid-size random files with adversarial small boxes: every
    # combination of dims in 1..5 plus random parity-mixed boxes.
    for x in range(1, 6):
        for y in range(1, 6):
            for z in range(1, 6):
                lines.append((x, y, z))
    for _ in range(rng.randint(50, 500)):
        lines.append(
            (
                rng.randint(1, 100),
                rng.randint(1, 100),
                rng.randint(1, 100),
            )
        )
    rng.shuffle(lines)

sys.stdout.write("\n".join(f"{x} {y} {z}" for x, y, z in lines) + "\n")
