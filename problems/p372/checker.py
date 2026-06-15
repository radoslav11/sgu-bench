"""Checker for p372: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility ("Impossible") is deterministic and must match the
reference. A plan is K distinct tea indices; round i serves K-i+1
guests, so its cost is sum (K-i+1)*c[plan_i]. The plan must avoid three
consecutive rounds of the same kind and its cost must equal the cost of
the reference's plan; the indices themselves may differ.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    k, n = int(in_tokens[0]), int(in_tokens[1])
    cost = [0] * (n + 1)
    kind = [0] * (n + 1)
    for i in range(1, n + 1):
        cost[i] = int(in_tokens[2 * i])
        kind[i] = int(in_tokens[2 * i + 1])

    expected_impossible = ans_tokens[0].lower() == "impossible"

    if not out_tokens:
        reject("empty output")
    if out_tokens[0].lower() == "impossible":
        if len(out_tokens) > 1:
            reject("trailing output after 'Impossible'")
        if expected_impossible:
            sys.exit(0)
        reject("a valid plan exists, got 'Impossible'")
    if expected_impossible:
        reject("expected 'Impossible'")

    if len(out_tokens) != k:
        reject(f"expected {k} tea indices, got {len(out_tokens)} tokens")
    try:
        plan = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer tea index")
    if any(not (1 <= t <= n) for t in plan):
        reject("tea index out of range")
    if len(set(plan)) != k:
        reject("tea types are not distinct")
    for i in range(k - 2):
        if kind[plan[i]] == kind[plan[i + 1]] == kind[plan[i + 2]]:
            reject(f"rounds {i + 1}..{i + 3} all use the same kind")

    ref_plan = [int(t) for t in ans_tokens]
    ref_cost = sum((k - i) * cost[ref_plan[i]] for i in range(k))
    out_cost = sum((k - i) * cost[plan[i]] for i in range(k))
    if out_cost != ref_cost:
        reject(f"plan costs {out_cost}, optimum is {ref_cost}")

    sys.exit(0)


if __name__ == "__main__":
    main()
