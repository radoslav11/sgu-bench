"""Tests for p340 (TeX2HTML): formulas in $...$, file size < 128 KB.

Formulas contain letters, digits, parens, spaces, + - * /, and one-level
^ / _ scripts whose body is a single character or a braced sub-formula.
Seed 1 is the sample-like minimal case; seed 2 fills ~120 KB with many
random formulas; seed 3 is one huge line dominated by deeply nested
braced scripts. Later seeds mix sizes and space placement.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
DIGITS = "0123456789"


def atom(depth, max_depth):
    r = rng.random()
    if r < 0.45 or depth >= max_depth:
        return "".join(rng.choice(LETTERS) for _ in range(rng.randint(1, 3)))
    if r < 0.75:
        return "".join(rng.choice(DIGITS) for _ in range(rng.randint(1, 3)))
    return "(" + formula(depth + 1, max_depth) + ")"


def operand(depth, max_depth):
    s = atom(depth, max_depth)
    if rng.random() < 0.45:
        mark = rng.choice("^_")
        if depth >= max_depth or rng.random() < 0.5:
            s += mark + rng.choice(LETTERS + DIGITS)
        else:
            s += mark + "{" + formula(depth + 1, max_depth) + "}"
    return s


def formula(depth, max_depth):
    parts = [operand(depth, max_depth)]
    for _ in range(rng.randint(0, 2 if depth else 5)):
        parts.append(rng.choice("+-*/"))
        parts.append(operand(depth, max_depth))
    return "".join(parts)


def spaced(body, space_prob):
    # Insert random spaces, but never immediately after '^' or '_'
    # (the statement guarantees a non-whitespace char follows them).
    out = []
    for ch in body:
        out.append(ch)
        if ch not in "^_" and rng.random() < space_prob:
            out.append(" " * rng.randint(1, 2))
    return "".join(out)


def line(max_depth=3, space_prob=0.1):
    return "$" + spaced(formula(0, max_depth), space_prob) + "$"


if seed == 1:
    print("$a$")
    print("$a - (v - b)$")
    print("$a_i + b_2$")
    print("$B^{k_i}$")
elif seed == 2:
    # Maximal: many random formulas, ~120 KB total.
    total = 0
    while total < 120 * 1000:
        s = line(max_depth=4, space_prob=0.15)
        print(s)
        total += len(s) + 1
elif seed == 3:
    # Maximal: a few huge lines with deeply nested braced scripts.
    chunks = []
    depth = 16000
    core = "x"
    body = []
    for _ in range(depth):
        body.append(rng.choice(LETTERS) + rng.choice("^_") + "{")
    body.append(core)
    body.append("}" * depth)
    print("$" + "".join(body) + "$")
    total = 0
    while total < 55 * 1000:
        s = line(max_depth=6, space_prob=0.05)
        print(s)
        total += len(s) + 1
else:
    n_lines = rng.randint(1, 40)
    for _ in range(n_lines):
        print(line(max_depth=rng.randint(1, 5), space_prob=rng.random() * 0.3))
