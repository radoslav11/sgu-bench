"""Checker for p158: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximal sum is deterministic and must match the reference's value.
The train position S is "output any one", so it is validated against
the input: S must keep all doors on the platform and the sum of
passenger-to-nearest-door distances at S must equal the optimal sum.
"""

import sys

EPS = 1e-4


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    l = int(in_tokens[pos]); pos += 1
    m = int(in_tokens[pos]); pos += 1
    p = [int(in_tokens[pos + i]) for i in range(m)]
    pos += m
    n = int(in_tokens[pos]); pos += 1
    d = [0] + [int(in_tokens[pos + i]) for i in range(n - 1)]

    best = float(ans_tokens[1])

    if len(out_tokens) != 2:
        reject(f"expected exactly 2 numbers, got {len(out_tokens)} tokens")
    try:
        s = float(out_tokens[0])
        claimed = float(out_tokens[1])
    except ValueError:
        reject("non-numeric output")

    if not (-EPS <= s <= l - d[-1] + EPS):
        reject(f"train position {s} is off the platform (0..{l - d[-1]})")
    if abs(claimed - best) > EPS:
        reject(f"optimal sum is {best}, claimed {claimed}")

    total = 0.0
    for pi in p:
        total += min(abs(dj + s - pi) for dj in d)
    if abs(total - best) > EPS:
        reject(f"sum at S={s} is {total}, not the optimal {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
