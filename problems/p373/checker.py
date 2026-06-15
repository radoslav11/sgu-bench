"""Checker for p373: usage checker.py <in> <out> <ans>; exit 0 = accept.

Two areas, each accepted within ABSOLUTE 1e-4 of the reference value, as
the statement specifies (the generic relative float checker would be far
too lenient on areas of magnitude ~3*10^4).
"""

import math
import sys

TOL = 1.05e-4


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    if len(out_tokens) != 2:
        reject(f"expected 2 numbers, got {len(out_tokens)} tokens")

    for i, (got, exp) in enumerate(zip(out_tokens, ans_tokens)):
        try:
            g = float(got)
        except ValueError:
            reject(f"token {i + 1}: '{got}' is not a number")
        if math.isnan(g) or math.isinf(g):
            reject(f"token {i + 1}: nan/inf")
        e = float(exp)
        if abs(g - e) > TOL:
            reject(f"token {i + 1}: {got} vs {exp} (abs tol 1e-4)")

    sys.exit(0)


if __name__ == "__main__":
    main()
