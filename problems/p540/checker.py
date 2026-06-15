"""Checker for p540: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum number of switched lights is deterministic and must equal the
reference's. Among speeds achieving that minimum the statement asks for
the maximum, so the contestant's v0 must match the reference's v0 within a
tolerance and lie in [vmin, vmax]. The listed lights must be exactly the
ones that are red when passing at the contestant's v0, and there must be
'count' of them.
"""

import math
import sys

EPS_V = 1e-5
EPS_PHASE = 1e-6


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    n = int(next(it))
    s = int(next(it))
    vmin = int(next(it))
    vmax = int(next(it))
    x = [0] * n
    r = [0] * n
    g = [0] * n
    d = [0] * n
    for i in range(n):
        x[i] = int(next(it))
        r[i] = int(next(it))
        g[i] = int(next(it))
        d[i] = int(next(it))

    ref_v = float(ans_tokens[0])
    ref_cnt = int(ans_tokens[1])

    if len(out_tokens) < 2:
        reject("output must contain at least v0 and the count")
    try:
        v0 = float(out_tokens[0])
    except ValueError:
        reject(f"v0 is not a number: '{out_tokens[0]}'")
    try:
        cnt = int(out_tokens[1])
    except ValueError:
        reject(f"count is not an integer: '{out_tokens[1]}'")

    if not (vmin - EPS_V <= v0 <= vmax + EPS_V):
        reject(f"v0={v0} outside [vmin, vmax]=[{vmin}, {vmax}]")

    if cnt != ref_cnt:
        reject(f"minimum switched count is {ref_cnt}, got {cnt}")

    if abs(v0 - ref_v) > EPS_V * max(1.0, abs(ref_v)):
        reject(
            f"v0 must be the maximum optimal speed {ref_v}, got {v0}"
        )

    switched = [int(t) for t in out_tokens[2:]]
    if len(switched) != cnt:
        reject(f"declared {cnt} switched lights, listed {len(switched)}")
    if len(set(switched)) != len(switched):
        reject("duplicate light index in switched list")
    if any(not (1 <= idx <= n) for idx in switched):
        reject("switched light index out of range")
    switched_set = set(switched)

    # Compute the set of lights that are red when passing at v0.
    red = set()
    for i in range(n):
        t = x[i] / v0
        c = r[i] + g[i]
        rem = math.fmod(t - d[i] + g[i], c)
        if rem < -EPS_PHASE:
            rem += c
        if rem > g[i] + EPS_PHASE and rem < c - EPS_PHASE:
            red.add(i + 1)

    if switched_set != red:
        missing = sorted(red - switched_set)
        extra = sorted(switched_set - red)
        reject(
            "switched set does not match the red lights at v0 "
            f"(missing red {missing}, extra {extra})"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
