"""Tests for p139 (Help Needed!): 4x4 fifteen-puzzle solvability.

Input is always a 4x4 permutation of 0..15, so every test is already
maximal-size. Seed 1 is the solved board, seed 2 a board scrambled by
legal moves (always YES), seed 3 the same scramble with two tiles
swapped (always NO); later seeds alternate scrambles and uniformly
random permutations.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed if seed != 3 else 2)

TARGET = list(range(1, 16)) + [0]


def scramble(rng, moves):
    board = TARGET[:]
    pos = 15
    for _ in range(moves):
        r, c = divmod(pos, 4)
        options = []
        if r > 0:
            options.append(pos - 4)
        if r < 3:
            options.append(pos + 4)
        if c > 0:
            options.append(pos - 1)
        if c < 3:
            options.append(pos + 1)
        nxt = rng.choice(options)
        board[pos], board[nxt] = board[nxt], board[pos]
        pos = nxt
    return board


if seed == 1:
    board = TARGET[:]
elif seed in (2, 3):
    board = scramble(rng, 500)
    if seed == 3:
        # swap two non-blank tiles: flips parity, becomes unsolvable
        i, j = [k for k in range(16) if board[k] != 0][:2]
        board[i], board[j] = board[j], board[i]
elif seed % 2 == 0:
    board = scramble(rng, rng.randint(1, 1000))
else:
    board = list(range(16))
    rng.shuffle(board)

for r in range(4):
    print(" ".join(str(x) for x in board[4 * r : 4 * r + 4]))
