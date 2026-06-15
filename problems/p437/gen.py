"""Tests for p437 (Hexodoku): K in [7,31], N the rank of the wanted
solution, and 31 cell prefills (0 = empty).

The search fills cells 1..31 in order, trying numbers 1..K, counting
complete colorings until the N-th. Cost is highest with the FEWEST colors
(K = 7, the most-constrained board, the largest branching depth) and a
large N, and with the empty board (no prefills pruning the tree). So the
maximal seeds use K = 7 on the empty board with large N, including the
"No way" boundary (N beyond the number of solutions). Seed 1 is the
first solution for a small K.

Prefilled boards are taken from a known valid full solution (the sample's
K=8 solution) with cells blanked out, guaranteeing the prefill is
rule-consistent as the statement requires.
"""

import random
import sys

# A known valid full solution for K = 8 (from the statement's sample).
SAMPLE_SOLUTION = [
    1, 2, 1, 3, 4, 5, 2, 2, 4, 6, 7, 1, 3, 7, 5, 1,
    8, 6, 2, 1, 3, 4, 5, 7, 8, 6, 7, 2, 3, 5, 8,
]
NUM_CELLS = 31


def emit(k, n, board):
    print(k, n)
    print(" ".join(map(str, board)))


seed = int(sys.argv[1])
rng = random.Random(seed)
empty = [0] * NUM_CELLS

if seed == 1:
    # First solution for a small K.
    emit(8, 1, empty)
    sys.exit()

if seed == 2:
    # Maximal search: fewest colors, empty board, large N (deep enumeration).
    emit(7, 10 ** 12, empty)
    sys.exit()

if seed == 3:
    # Maximal "No way": K = 7 empty board, N far beyond the solution count,
    # forcing the search to enumerate every solution before giving up.
    emit(7, 10 ** 18, empty)
    sys.exit()

if seed == 4:
    # Largest K, first solution.
    emit(31, 1, empty)
    sys.exit()

if seed == 5:
    # Prefilled, rule-consistent board (sample solution with ~half blanked).
    board = list(SAMPLE_SOLUTION)
    for i in range(NUM_CELLS):
        if rng.random() < 0.5:
            board[i] = 0
    emit(8, rng.randint(1, 50), board)
    sys.exit()

# Mixed: random K, random N, and either an empty or a lightly-prefilled board.
k = rng.randint(7, 31)
n = rng.randint(1, 10 ** 9)
if rng.random() < 0.5:
    board = empty
else:
    board = list(SAMPLE_SOLUTION)
    for i in range(NUM_CELLS):
        if rng.random() < 0.7:
            board[i] = 0
    k = 8  # keep prefill consistent with the K=8 solution
emit(k, n, board)
