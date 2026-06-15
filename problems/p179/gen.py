"""Tests for p179 (Brackets light): next balanced bracket sequence.

Input is one correct bracket sequence of length <= 10000. Seed 1 is the
minimal "()", seed 2 the lexicographically largest max-length sequence
("()" * 5000, answer "No solution"), seed 3 a fully nested max-length
sequence. Later seeds are random balanced sequences of various lengths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_LEN = 10000


def random_balanced(n, rng, bias=0.5):
    half = n // 2
    open_used = 0
    balance = 0
    out = []
    for i in range(n):
        remaining = n - i
        can_open = open_used < half
        can_close = balance > 0
        # must close if balance equals remaining positions
        if balance == remaining:
            can_open = False
        if can_open and (not can_close or rng.random() < bias):
            out.append("(")
            open_used += 1
            balance += 1
        else:
            out.append(")")
            balance -= 1
    return "".join(out)


if seed == 1:
    print("()")
elif seed == 2:
    print("()" * (MAX_LEN // 2))
elif seed == 3:
    print("(" * (MAX_LEN // 2) + ")" * (MAX_LEN // 2))
elif seed == 4:
    # max length, deep random nesting (bias towards opening)
    print(random_balanced(MAX_LEN, rng, bias=0.7))
elif seed == 5:
    # almost-largest sequence: long "()" tail after a small head
    print("(())" + "()" * ((MAX_LEN - 4) // 2))
else:
    n = 2 * rng.randint(1, MAX_LEN // 2)
    print(random_balanced(n, rng, bias=rng.uniform(0.4, 0.6)))
