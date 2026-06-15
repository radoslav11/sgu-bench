"""Tests for p517 (Cornerless Tiling): 1 <= n, m <= 1000.

The count is a deterministic big integer. Seed 1 is the minimal 1x1 case,
seeds 2 and 3 are the maximal 1000x1000 board and a maximal-but-thin board
(2x1000 and odd-dimension 999x1000) so the big-integer growth is exercised.
The remaining seeds cover the structural cases: n=1, n=2, odd small,
even small, odd area (answer 0), and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(m, n):
    print(m, n)


if seed == 1:
    emit(1, 1)
elif seed == 2:
    emit(1000, 1000)
elif seed == 3:
    emit(2, 1000)
elif seed == 4:
    emit(999, 1000)
elif seed == 5:
    emit(1, 1000)
elif seed == 6:
    emit(3, 1000)
elif seed == 7:
    emit(1000, 999)
elif seed == 8:
    # Odd area -> answer 0.
    emit(3, 3)
elif seed == 9:
    emit(4, 4)
else:
    m = rng.randint(1, 1000)
    n = rng.randint(1, 1000)
    emit(m, n)
