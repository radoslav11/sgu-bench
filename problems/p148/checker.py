"""Checker for p148: usage checker.py <in> <out> <ans>; exit 0 = accept.

The output is a set of level numbers to pay for. It is accepted when
simulating the cascade (paid levels burst; a level also bursts when the
accumulated water strictly exceeds its limit, pouring everything down)
ends with level N depressurized, and the total paid cost equals the
reference's optimal cost. Any optimal set is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    w = [0] * (n + 1)
    l = [0] * (n + 1)
    p = [0] * (n + 1)
    for i in range(1, n + 1):
        w[i] = int(in_tokens[3 * i - 2])
        l[i] = int(in_tokens[3 * i - 1])
        p[i] = int(in_tokens[3 * i])

    ref_cost = sum(p[int(t)] for t in ans_tokens)

    try:
        chosen = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")
    if any(not (1 <= c <= n) for c in chosen):
        reject("level number out of range")
    if len(set(chosen)) != len(chosen):
        reject("duplicate level in output")

    paid = set(chosen)
    cost = sum(p[i] for i in paid)
    if cost != ref_cost:
        reject(f"cost {cost}, optimal is {ref_cost}")

    flow = 0
    burst_last = False
    for i in range(1, n + 1):
        water = flow + w[i]
        if i in paid or water > l[i]:
            flow = water
            burst_last = i == n
        else:
            flow = 0

    if not burst_last:
        reject("level N does not get depressurized")

    sys.exit(0)


if __name__ == "__main__":
    main()
