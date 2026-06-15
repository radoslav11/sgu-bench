"""Checker for p236: usage checker.py <in> <out> <ans>; exit 0 = accept.

"0" (no cycle) is deterministic and must match the reference. Otherwise
the output must be a closed walk of 2..50 towns over existing routes;
its exact rational cost/time ratio must be at least the reference
cycle's (compared with cross-multiplication, no floating point).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_cycle(tokens, n, route, label):
    if not tokens:
        reject(f"{label}: empty")
    try:
        k = int(tokens[0])
    except ValueError:
        reject(f"{label}: first token must be an integer")
    if k == 0:
        return None
    if not (2 <= k <= 50):
        reject(f"{label}: K = {k} out of range 2..50")
    if len(tokens) != 1 + k:
        reject(f"{label}: expected {k} towns, got {len(tokens) - 1}")
    try:
        towns = [int(x) for x in tokens[1:]]
    except ValueError:
        reject(f"{label}: non-integer town")
    if any(not (1 <= v <= n) for v in towns):
        reject(f"{label}: town index out of range")
    total_c = total_t = 0
    for i in range(k):
        a, b = towns[i], towns[(i + 1) % k]
        if (a, b) not in route:
            reject(f"{label}: no route {a} -> {b}")
        c, t = route[(a, b)]
        total_c += c
        total_t += t
    return total_c, total_t


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    route = {}
    for i in range(m):
        a, b, c, t = (int(x) for x in in_tokens[2 + 4 * i:6 + 4 * i])
        route[(a, b)] = (c, t)

    ans = parse_cycle(ans_tokens, n, route, "answer file")
    out = parse_cycle(out_tokens, n, route, "output")

    if ans is None:
        if out is not None:
            reject("expected '0' (no cycle exists)")
        sys.exit(0)
    if out is None:
        reject("output '0' but a cycle exists")

    out_c, out_t = out
    ans_c, ans_t = ans
    if out_c * ans_t < ans_c * out_t:
        reject(
            f"ratio {out_c}/{out_t} is below the optimal {ans_c}/{ans_t}"
        )
    sys.exit(0)


if __name__ == "__main__":
    main()
