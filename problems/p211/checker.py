"""Checker for p211: usage checker.py <in> <out> <ans>; exit 0 = accept.

Simulates the claimed register changes: registers start at 0, each
operation may set 1..4 distinct registers to values in {0,1,2}, and the
net value change sum (new-old)*2^reg must equal the 2^i being added.
Any valid simulation is accepted; the reference output is not consulted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n = int(in_tokens[0])
    m = int(in_tokens[1])
    adds = [int(t) for t in in_tokens[2:2 + m]]

    reg = [0] * n
    pos = 0
    for op, i in enumerate(adds, start=1):
        if pos >= len(out_tokens):
            reject(f"operation {op}: output ended early")
        try:
            k = int(out_tokens[pos]); pos += 1
            if not (1 <= k <= 4):
                reject(f"operation {op}: K={k} not in [1, 4]")
            changes = []
            for _ in range(k):
                r = int(out_tokens[pos]); v = int(out_tokens[pos + 1])
                pos += 2
                changes.append((r, v))
        except (ValueError, IndexError):
            reject(f"operation {op}: malformed change list")

        seen = set()
        delta = 0
        for r, v in changes:
            if not (0 <= r < n):
                reject(f"operation {op}: register {r} out of range")
            if v not in (0, 1, 2):
                reject(f"operation {op}: value {v} not in {{0,1,2}}")
            if r in seen:
                reject(f"operation {op}: register {r} changed twice")
            seen.add(r)
            delta += (v - reg[r]) << r
            reg[r] = v

        if delta != (1 << i):
            reject(
                f"operation {op}: counter changed by {delta}, "
                f"expected 2^{i}"
            )

    if pos != len(out_tokens):
        reject("trailing output after last operation")

    sys.exit(0)


if __name__ == "__main__":
    main()
