"""Tests for p324 (The Text Formatting).

N <= 100 quoted documents, each 1..100 characters of Latin letters,
spaces and '-'. Any such string is a legal document, so lines are built
from weighted character soup plus hand-crafted patterns: lone dashes,
boundary dashes (error cases), hyphenated words, dash chains split by
spaces, all-space and all-dash lines.

Seed 1 is one single-letter document, seeds 2-3 are 100 lines x 100
chars (word-heavy vs dash-heavy).
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

SPECIALS = [
    "-",
    "--",
    "---",
    " - ",
    " -- ",
    "a-",
    "-a",
    "a-b",
    "a - b",
    "a  -  -  b",
    "a -- b -- ",
    " ",
    "   ",
    "----- word -----",
    "x" * 100,
    "-" * 100,
    "- " * 50,
]


def soup(length, dash_w, space_w):
    chars = []
    while len(chars) < length:
        r = rng.random()
        if r < dash_w:
            chars.append("-")
        elif r < dash_w + space_w:
            chars.append(" ")
        else:
            chars.append(rng.choice(string.ascii_letters))
    return "".join(chars)


if seed == 1:
    print(1)
    print('"a"')
    sys.exit()

lines = []
if seed == 2:
    for _ in range(100):
        lines.append(soup(100, 0.12, 0.18))
elif seed == 3:
    for i in range(100):
        if i % 4 == 0:
            lines.append(soup(100, 0.55, 0.25))
        elif i % 4 == 1:
            lines.append(("-" * rng.randint(1, 6) + " ") * 14 + "ab")
        elif i % 4 == 2:
            lines.append(soup(100, 0.35, 0.05))
        else:
            lines.append(rng.choice(SPECIALS))
else:
    n = rng.randint(1, 100)
    for _ in range(n):
        if rng.random() < 0.4:
            lines.append(rng.choice(SPECIALS))
        else:
            lines.append(
                soup(rng.randint(1, 100), rng.uniform(0, 0.5), rng.uniform(0, 0.4))
            )

lines = [s[:100] if len(s) > 100 else s for s in lines]
print(len(lines))
for s in lines:
    print(f'"{s}"')
