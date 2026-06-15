"""Checker for p323: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum tax P is unique and must match the reference. The kept
airline R and the list of sold flights are a certificate: the sold
flights must be distinct valid flight indices whose taxes sum to P, and
airline R's own flights plus the sold ones must connect all N towns.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, k = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    flights = []
    for i in range(k):
        a = int(in_tokens[3 + 4 * i])
        b = int(in_tokens[4 + 4 * i])
        c = int(in_tokens[5 + 4 * i])
        p = int(in_tokens[6 + 4 * i])
        flights.append((a, b, c, p))

    expected_cost = int(ans_tokens[0])

    if len(out_tokens) < 3:
        reject("expected at least 'P R Q'")
    try:
        cost = int(out_tokens[0])
        airline = int(out_tokens[1])
        q = int(out_tokens[2])
        sold = [int(x) for x in out_tokens[3:]]
    except ValueError:
        reject("non-integer token in output")

    if cost != expected_cost:
        reject(f"minimum cost is {expected_cost}, got {cost}")
    if not (1 <= airline <= m):
        reject(f"airline {airline} out of range 1..{m}")
    if q < 0 or len(sold) != q:
        reject(f"declared {q} operations, listed {len(sold)}")
    if any(not (1 <= i <= k) for i in sold):
        reject("flight index out of range")
    if len(set(sold)) != q:
        reject("duplicate flight index in operations")

    paid = sum(flights[i - 1][3] for i in sold)
    if paid != cost:
        reject(f"listed operations cost {paid}, claimed {cost}")

    parent = list(range(n + 1))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    def union(x, y):
        parent[find(x)] = find(y)

    for a, b, c, p in flights:
        if c == airline:
            union(a, b)
    for i in sold:
        union(flights[i - 1][0], flights[i - 1][1])

    root = find(1)
    for v in range(2, n + 1):
        if find(v) != root:
            reject(f"town {v} not reachable by airline {airline}")

    sys.exit(0)


if __name__ == "__main__":
    main()
