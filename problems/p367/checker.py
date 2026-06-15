"""Checker for p367: usage checker.py <in> <out> <ans>; exit 0 = accept.

The solved-problem count and the minimal penalty are deterministic and
must match the reference. The order itself is a certificate: it must list
that many distinct problems, fit into the training duration, respect all
prerequisite constraints, and its penalty must equal the claimed one.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, total_time = int(in_tokens[0]), int(in_tokens[1])
    t = [0] + [int(x) for x in in_tokens[2:2 + n]]
    m = int(in_tokens[2 + n])
    cons = in_tokens[3 + n:]

    exp_cnt, exp_pen = int(ans_tokens[0]), int(ans_tokens[1])

    if len(out_tokens) < 2:
        reject("missing count/penalty line")
    try:
        got_cnt, got_pen = int(out_tokens[0]), int(out_tokens[1])
    except ValueError:
        reject("count/penalty are not integers")
    if got_cnt != exp_cnt:
        reject(f"solved count is {exp_cnt}, got {got_cnt}")
    if got_pen != exp_pen:
        reject(f"minimal penalty is {exp_pen}, got {got_pen}")

    order = out_tokens[2:]
    if len(order) != got_cnt:
        reject(f"declared {got_cnt} problems, listed {len(order)}")
    try:
        order = [int(x) for x in order]
    except ValueError:
        reject("non-integer problem number in order")
    if any(not (1 <= p <= n) for p in order):
        reject("problem number out of range")
    if len(set(order)) != len(order):
        reject("duplicate problem in order")

    pos = {p: i for i, p in enumerate(order)}
    elapsed = 0
    penalty = 0
    for p in order:
        elapsed += t[p]
        penalty += elapsed
    if elapsed > total_time:
        reject(f"schedule takes {elapsed} > T = {total_time}")
    if penalty != got_pen:
        reject(f"order has penalty {penalty}, claimed {got_pen}")

    for i in range(m):
        a, b = int(cons[2 * i]), int(cons[2 * i + 1])
        if b in pos:
            if a not in pos:
                reject(f"problem {b} solved without prerequisite {a}")
            if pos[a] > pos[b]:
                reject(f"prerequisite {a} solved after {b}")

    sys.exit(0)


if __name__ == "__main__":
    main()
