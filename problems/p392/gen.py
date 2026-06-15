"""Tests for p392 (Cyclic Troubles): R, C <= 30, Q <= 50, compressed
query length <= 2000, decompressed length <= 10^9.

YES queries are built by simulating the deterministic walk from a cell:
walks that fall into a cycle become tail + (cycle)K + partial with K
pushed close to the 10^9 decompressed limit; exiting walks give plain
words. NO queries are mutated YES words, oversized repeats and random
strings. Arrows are '<' '>' '^' 'v' as in the sample.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_DECOMP = 10**9
ARROWS = "<>^v"


def next_cell(arrows, R, C, x, y):
    a = arrows[x][y]
    if a == '<':
        y -= 1
    elif a == '>':
        y += 1
    elif a == '^':
        x -= 1
    else:
        x += 1
    if 0 <= x < R and 0 <= y < C:
        return (x, y)
    return None


def walk(arrows, letters, R, C, x, y):
    # Follow arrows until exiting or revisiting a cell.
    # Returns (word_list, cycle_start) with cycle_start None on exit.
    seen = {}
    word = []
    pos = (x, y)
    while pos is not None and pos not in seen:
        seen[pos] = len(word)
        word.append(letters[pos[0]][pos[1]])
        pos = next_cell(arrows, R, C, pos[0], pos[1])
    return word, (None if pos is None else seen[pos])


def yes_query(arrows, letters, R, C, x, y):
    word, cyc = walk(arrows, letters, R, C, x, y)
    if cyc is None:
        ln = rng.randint(1, len(word))
        return "".join(word[:ln])
    tail = "".join(word[:cyc])
    cycle = "".join(word[cyc:])
    kmax = (MAX_DECOMP - len(tail) - len(cycle)) // len(cycle)
    k = rng.choice([1, 2, rng.randint(3, kmax), kmax])
    part = "".join(cycle[:rng.randint(0, min(len(cycle), 80))])
    return f"{tail}({cycle}){k}{part}"


def mutate(qstr):
    # Flip one letter (never a paren or digit) -> usually NO.
    idxs = [i for i, ch in enumerate(qstr) if ch.islower()]
    i = rng.choice(idxs)
    repl = rng.choice([c for c in "ab" if c != qstr[i]])
    return qstr[:i] + repl + qstr[i + 1:]


def random_grid(R, C, alpha):
    arrows = [["".join(rng.choice(ARROWS) for _ in range(C))][0]
              for _ in range(R)]
    letters = ["".join(rng.choice(alpha) for _ in range(C))
               for _ in range(R)]
    return arrows, letters


def emit(R, C, arrows, letters, queries):
    out = [f"{R} {C}"] + list(arrows) + list(letters)
    out.append(str(len(queries)))
    out.extend(queries)
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, 1, [">"], ["a"], ["a"])
elif seed in (2, 3):
    R = C = 30
    if seed == 2:
        # Random arrow field, binary alphabet, 50 near-limit queries.
        arrows, letters = random_grid(R, C, "ab")
    else:
        # One Hamiltonian 900-cell cycle: every query walks the whole
        # grid, repeated ~10^6 times via compression.
        grid = [[''] * C for _ in range(R)]
        for i in range(R):
            grid[i][0] = '^'
        grid[0] = ['>'] * C
        grid[0][C - 1] = 'v'
        for i in range(1, R):
            right = (i % 2 == 1)
            for j in range(1, C):
                grid[i][j] = '<' if right else '>'
            if i < R - 1:
                grid[i][1 if right else C - 1] = 'v'
            else:
                grid[i][1] = '<'
        arrows = ["".join(row) for row in grid]
        letters = ["".join(rng.choice("ab") for _ in range(C))
                   for _ in range(R)]
    queries = []
    for qi in range(50):
        x, y = rng.randrange(R), rng.randrange(C)
        qstr = yes_query(arrows, letters, R, C, x, y)
        if qi % 3 == 2:
            qstr = mutate(qstr)
        queries.append(qstr)
    emit(R, C, arrows, letters, queries)
elif seed == 4:
    # All arrows point right: every walk exits; long repeats must be NO
    # purely because of length, plus genuine prefix matches.
    R, C = 30, 30
    arrows = [">" * C for _ in range(R)]
    letters = ["".join(rng.choice("ab") for _ in range(C))
               for _ in range(R)]
    queries = []
    for qi in range(50):
        x, y = rng.randrange(R), rng.randrange(C)
        if qi % 4 == 3:
            queries.append(f"({letters[x][y:] or 'a'})999999")
        else:
            queries.append(yes_query(arrows, letters, R, C, x, y))
    emit(R, C, arrows, letters, queries)
elif seed == 5:
    # Single-cell self-loop-like exits and a 1xC strip.
    C = 30
    arrows = ["<" * C]
    letters = ["".join(rng.choice("ab") for _ in range(C))]
    queries = []
    for qi in range(20):
        y = rng.randrange(C)
        q = yes_query(arrows, letters, 1, C, 0, y)
        queries.append(q if qi % 2 == 0 else mutate(q))
    queries.append("(a)1000000000")
    emit(1, C, arrows, letters, queries)
else:
    # Random mid-size grids, mixed alphabets and query styles.
    R = rng.randint(1, 30)
    C = rng.randint(1, 30)
    alpha = rng.choice(["ab", "abc", "abcdefghijklmnopqrstuvwxyz"])
    arrows, letters = random_grid(R, C, alpha)
    nq = rng.randint(1, 50)
    queries = []
    for qi in range(nq):
        x, y = rng.randrange(R), rng.randrange(C)
        kind = rng.random()
        if kind < 0.5:
            queries.append(yes_query(arrows, letters, R, C, x, y))
        elif kind < 0.8:
            queries.append(mutate(yes_query(arrows, letters, R, C, x, y)))
        else:
            ln = rng.randint(1, 30)
            w = "".join(rng.choice(alpha) for _ in range(ln))
            k = rng.randint(1, MAX_DECOMP // ln)
            queries.append(f"({w}){k}")
    emit(R, C, arrows, letters, queries)
