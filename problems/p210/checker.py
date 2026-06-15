"""Checker for p210: usage checker.py <in> <out> <ans>; exit 0 = accept.

Output is one girl number (or 0) per son. Non-zero girls must be distinct
and lie in that son's preference list. The objective sum of A_i^2 over
married sons must equal the reference assignment's value; any assignment
achieving it is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    n = int(in_tokens[pos]); pos += 1
    loves = [int(t) for t in in_tokens[pos:pos + n]]; pos += n
    prefs = []
    for _ in range(n):
        k = int(in_tokens[pos]); pos += 1
        prefs.append(set(int(t) for t in in_tokens[pos:pos + k])); pos += k

    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")
    try:
        match = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    ref_match = [int(t) for t in ans_tokens]
    ref_value = sum(
        loves[i] ** 2 for i in range(n) if ref_match[i] != 0
    )

    used = set()
    value = 0
    for i, g in enumerate(match):
        if g == 0:
            continue
        if not (1 <= g <= n):
            reject(f"son {i + 1}: girl {g} out of range")
        if g not in prefs[i]:
            reject(f"son {i + 1} does not like girl {g}")
        if g in used:
            reject(f"girl {g} married twice")
        used.add(g)
        value += loves[i] ** 2

    if value != ref_value:
        reject(f"objective {value} != optimal {ref_value}")

    sys.exit(0)


if __name__ == "__main__":
    main()
