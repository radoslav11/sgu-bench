"""Tests for p359 (Pointers). A program of N <= 30 lines over pointers
a, b, c, d with four operator types:
  p^:=digit;   p1^:=p2^;   p1:=p2;   writeln(p^);
No blanks inside lines (per the statement), trailing ';' on every line.

Seed plan:
- seed 1: N = 1, single writeln of an untouched cell.
- seed 2: N = 30 random mix of all four operators (maximal size).
- seed 3: N = 30 alias-heavy program: many p1:=p2 reassignments so all
  four pointers collapse onto shared cells, then writes and writelns.
- seed 4: N = 30, only writelns.
- seed 5: self-assignments (a:=a, a^:=a^) mixed with writelns.
- seeds 6+: random programs of random length.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

PTRS = "abcd"


def rand_op(rng, weights=(3, 3, 3, 3)):
    kind = rng.choices(range(4), weights=weights)[0]
    if kind == 0:
        return f"{rng.choice(PTRS)}^:={rng.randint(0, 9)};"
    if kind == 1:
        return f"{rng.choice(PTRS)}^:={rng.choice(PTRS)}^;"
    if kind == 2:
        return f"{rng.choice(PTRS)}:={rng.choice(PTRS)};"
    return f"writeln({rng.choice(PTRS)}^);"


if seed == 1:
    lines = ["writeln(d^);"]
elif seed == 2:
    lines = [rand_op(rng) for _ in range(30)]
elif seed == 3:
    lines = []
    for _ in range(18):
        lines.append(f"{rng.choice(PTRS)}:={rng.choice(PTRS)};")
    for _ in range(6):
        lines.append(f"{rng.choice(PTRS)}^:={rng.randint(0, 9)};")
    for _ in range(6):
        lines.append(f"writeln({rng.choice(PTRS)}^);")
elif seed == 4:
    lines = [f"writeln({rng.choice(PTRS)}^);" for _ in range(30)]
elif seed == 5:
    lines = []
    for _ in range(10):
        p = rng.choice(PTRS)
        lines.append(f"{p}:={p};")
        lines.append(f"{p}^:={p}^;")
        lines.append(f"writeln({p}^);")
else:
    n = rng.randint(2, 30)
    lines = [rand_op(rng) for _ in range(n)]

print(len(lines))
for line in lines:
    print(line)
