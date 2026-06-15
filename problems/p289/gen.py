"""Tests for p289 (Challenging Tic-Tac-Toe): 3x3 boards of X/0/.

Each test case is 3 board lines plus a blank line; the file ends with
"Qc". There are only 3^9 = 19683 distinct boards and the statement bans
repeats, so seeds 2-3 simply feed all of them in two different shuffled
orders. Other seeds sample random subsets (legal, illegal, terminal
positions all appear naturally).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

CHARS = ".X0"


def board_lines(state):
    cells = []
    for i in range(9):
        cells.append(CHARS[state % 3])
        state //= 3
    return [
        "".join(cells[0:3]),
        "".join(cells[3:6]),
        "".join(cells[6:9]),
    ]


if seed == 1:
    states = [0]
elif seed in (2, 3):
    states = list(range(3 ** 9))
    rng.shuffle(states)
else:
    states = rng.sample(range(3 ** 9), rng.randint(50, 5000))

for state in states:
    for line in board_lines(state):
        print(line)
    print()
print("Qc")
