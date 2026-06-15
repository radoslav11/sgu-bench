"""Checker for p171: usage checker.py <in> <out> <ans>; exit 0 = accept.

Many assignments achieve the optimum, so the output is judged as a
certificate: exactly N zone indices, zone i receiving exactly N[i]
students, and the total weight of students whose level beats their
zone's level must equal the reference assignment's total. The actual
zone numbers need not match the reference.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def value(assign, ps, ws, qs):
    return sum(w for z, p, w in zip(assign, ps, ws) if p > qs[z - 1])


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    k = int(next(it))
    caps = [int(next(it)) for _ in range(k)]
    n = sum(caps)
    qs = [int(next(it)) for _ in range(k)]
    ps = [int(next(it)) for _ in range(n)]
    ws = [int(next(it)) for _ in range(n)]

    if len(out_tokens) != n:
        reject(f"expected {n} zone indices, got {len(out_tokens)}")
    try:
        assign = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer zone index")
    if any(not (1 <= z <= k) for z in assign):
        reject("zone index out of range")

    used = [0] * (k + 1)
    for z in assign:
        used[z] += 1
    for i in range(k):
        if used[i + 1] != caps[i]:
            reject(f"zone {i + 1} got {used[i + 1]} students, "
                   f"capacity is {caps[i]}")

    ref_assign = [int(t) for t in ans_tokens]
    best = value(ref_assign, ps, ws, qs)
    got = value(assign, ps, ws, qs)
    if got != best:
        reject(f"total qualifying weight {got}, optimum is {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
