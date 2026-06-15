"""Checker for p114: usage checker.py <in> <out> <ans>; exit 0 = accept.

The displeasure sum is piecewise linear in the station position, so the
set of minimizers is an interval [lo, hi] whose endpoints are city
coordinates (the weighted-median set, computed exactly from the input
with integer arithmetic). Any output within 1e-4 of that interval is
accepted; the reference's own position is not compared against.
"""

import sys

TOL = 1e-4


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n = int(in_tokens[0])
    cities = []
    for i in range(n):
        x = int(in_tokens[1 + 2 * i])
        p = int(in_tokens[2 + 2 * i])
        cities.append((x, p))
    cities.sort()

    total = sum(p for _, p in cities)
    # lo: smallest coordinate with 2*prefix >= total
    # hi: largest coordinate with 2*suffix >= total
    lo = hi = None
    acc = 0
    for x, p in cities:
        acc += p
        if 2 * acc >= total:
            lo = x
            break
    acc = 0
    for x, p in reversed(cities):
        acc += p
        if 2 * acc >= total:
            hi = x
            break

    if len(out_tokens) != 1:
        reject(f"expected exactly one number, got {len(out_tokens)} tokens")
    try:
        pos = float(out_tokens[0])
    except ValueError:
        reject(f"not a number: '{out_tokens[0]}'")

    if not (lo - TOL <= pos <= hi + TOL):
        reject(f"position {pos} outside optimal interval [{lo}, {hi}]")

    sys.exit(0)


if __name__ == "__main__":
    main()
