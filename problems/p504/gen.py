"""Tests for p504 (Square Palindrome): largest k x k palindrome square in
an h x w grid, 1 <= h, w <= 700, lowercase letters.

Seed 1 is the 1x1 minimum. Seeds 2 and 3 are full 700x700 grids: seed 2
is all one letter (the whole grid is one giant palindrome square, the
heaviest case for the size search), seed 3 is uniform random over 26
letters (tiny answer but every binary-search level still scans the grid).
Later seeds mix small alphabets, near-palindromic blocks and rectangular
shapes that stress partial palindrome runs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = "abcdefghijklmnopqrstuvwxyz"


def emit(grid):
    h = len(grid)
    w = len(grid[0]) if h else 0
    out = [f"{h} {w}"]
    out.extend(grid)
    sys.stdout.write("\n".join(out) + "\n")


def rand_grid(h, w, alphabet):
    k = len(alphabet)
    return [
        "".join(alphabet[rng.randrange(k)] for _ in range(w))
        for _ in range(h)
    ]


if seed == 1:
    emit([rng.choice(LETTERS)])

elif seed == 2:
    ch = rng.choice(LETTERS)
    emit([ch * 700 for _ in range(700)])

elif seed == 3:
    emit(rand_grid(700, 700, LETTERS))

elif seed == 4:
    # 700x700 over a two-letter alphabet: many short palindromes,
    # forcing the search to push deeper before failing.
    emit(rand_grid(700, 700, "ab"))

elif seed == 5:
    # Tall and thin random grid.
    emit(rand_grid(700, 3, LETTERS))

elif seed == 6:
    # Short and wide random grid over a small alphabet.
    emit(rand_grid(2, 700, "abc"))

elif seed == 7:
    # A big block of one letter embedded in random noise, so a large
    # palindrome square exists but is not the whole grid.
    h = w = 700
    grid = [list(rand_grid(1, w, LETTERS)[0]) for _ in range(h)]
    bh = rng.randint(200, 400)
    bw = bh
    r0 = rng.randint(0, h - bh)
    c0 = rng.randint(0, w - bw)
    ch = rng.choice(LETTERS)
    for i in range(r0, r0 + bh):
        for j in range(c0, c0 + bw):
            grid[i][j] = ch
    emit(["".join(r) for r in grid])

elif seed == 8:
    # Rows that are individually palindromes but columns mostly are not.
    h = rng.randint(300, 700)
    w = rng.randint(300, 700)
    grid = []
    for _ in range(h):
        half = "".join(rng.choice("abcde") for _ in range(w // 2))
        mid = rng.choice("abcde") if w % 2 else ""
        grid.append(half + mid + half[::-1])
    emit(grid)

elif seed == 9:
    # Concentric-style structured grid: cell value depends on min distance
    # to a border, which makes both rows and columns symmetric.
    n = 501
    grid = []
    for i in range(n):
        row = []
        for j in range(n):
            d = min(i, n - 1 - i, j, n - 1 - j)
            row.append(LETTERS[d % 26])
        grid.append("".join(row))
    emit(grid)

else:
    h = rng.randint(1, 700)
    w = rng.randint(1, 700)
    alphabet = rng.choice([LETTERS, "ab", "abc", "abcd"])
    emit(rand_grid(h, w, alphabet))
