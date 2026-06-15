"""Checker for p459: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any valid dance is accepted: the output is parsed into +i / -i / ++i /
--i operations and simulated from an empty stage. Every operation must
be legal, the stage must stay non-empty and hold at most k dancers
between operations, the dance must end with an empty stage, and every
non-empty set of at most k dancers must appear exactly once.
"""

import re
import sys
from math import comb


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = "".join(open(sys.argv[2]).read().split())

    n, k = int(in_tokens[0]), int(in_tokens[1])

    if out_text == "0":
        reject("claimed no solution, but a dance always exists")
    if not out_text:
        reject("empty output")

    ops = []
    pos = 0
    token_re = re.compile(r"(\+\+|--|\+|-)(\d+)")
    while pos < len(out_text):
        m = token_re.match(out_text, pos)
        if not m:
            reject(f"cannot parse output at position {pos}")
        ops.append((m.group(1), int(m.group(2))))
        pos = m.end()

    stage = 0
    seen = set()
    expected_states = sum(comb(n, j) for j in range(1, k + 1))

    for idx, (op, i) in enumerate(ops, 1):
        if not (1 <= i <= n):
            reject(f"operation {idx}: dancer {i} out of range")
        bit = 1 << (i - 1)
        if op == "+":
            if stage & bit:
                reject(f"operation {idx}: dancer {i} is already on stage")
            stage |= bit
        elif op == "-":
            if not (stage & bit):
                reject(f"operation {idx}: dancer {i} is not on stage")
            stage &= ~bit
        else:
            j = i + 1 if op == "++" else i - 1
            if not (1 <= j <= n):
                reject(f"operation {idx}: dancer {j} does not exist")
            jbit = 1 << (j - 1)
            if not (stage & bit):
                reject(f"operation {idx}: dancer {i} is not on stage")
            if stage & jbit:
                reject(f"operation {idx}: dancer {j} is already on stage")
            stage = (stage & ~bit) | jbit

        if bin(stage).count("1") > k:
            reject(f"operation {idx}: more than {k} dancers on stage")
        if stage == 0:
            if idx != len(ops):
                reject(f"operation {idx}: stage is empty before the end")
        else:
            if stage in seen:
                reject(f"operation {idx}: set {stage:b} appears twice")
            seen.add(stage)

    if stage != 0:
        reject("dance does not end with an empty stage")
    if len(seen) != expected_states:
        reject(
            f"visited {len(seen)} sets, expected {expected_states} "
            f"(every non-empty set of at most {k} dancers exactly once)"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
