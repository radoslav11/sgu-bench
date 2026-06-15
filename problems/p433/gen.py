"""Tests for p433 (pavement tiling): N + M <= 20, K <= 5, lengths <= 1000.

Seed 1 is the 1x1 board. Seeds 2-3 are 10x10 boards with sparse length
sets {3, 8} and {4, 7} so neither side length is representable and the
column DP really has to run. Later seeds mix random boards, unusable
huge bricks, parity-impossible cases and thin 1 x W strips.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, lengths):
    print(n, m, len(lengths))
    print(*lengths)


if seed == 1:
    emit(1, 1, [1])
elif seed == 2:
    # Maximal board, neither 10 = 3a + 8b nor 10 representable; the
    # extra lengths are bigger than the board and unusable.
    emit(10, 10, [3, 8, 998, 999, 1000])
elif seed == 3:
    # Second maximal board with a different sparse set {4, 7}.
    emit(10, 10, [4, 7, 997, 996, 1000])
elif seed == 4:
    # Thin strip, width is a sum 4 + 5 + 5 + 5.
    emit(1, 19, [4, 5, 11, 13, 17])
elif seed == 5:
    # Only a length-2 brick and odd area -> NO.
    emit(9, 11, [2])
elif seed == 6:
    # All bricks longer than both sides -> NO.
    emit(10, 10, [11, 12, 500, 999, 1000])
elif seed == 7:
    # 1x1 board but no length-1 brick -> NO.
    emit(1, 1, [2, 3])
else:
    n = rng.randint(1, 19)
    m = rng.randint(max(1, 2 - n), 20 - n)
    k = rng.randint(1, 5)
    if rng.random() < 0.5:
        # Small usable lengths.
        lengths = rng.sample(range(1, 20), k)
    else:
        # Mix of small and huge (unusable) lengths.
        lengths = rng.sample(range(2, 11), min(k, 3))
        while len(lengths) < k:
            big = rng.randint(20, 1000)
            if big not in lengths:
                lengths.append(big)
    rng.shuffle(lengths)
    emit(n, m, lengths)
