"""Checker for p457: usage checker.py <in> <out> <ans>; exit 0 = accept.

The day count p is deterministic and must match the reference. The p
routes are a certificate: each must be a directed walk from A to B over
input roads, the total number of traversals of every road must not
exceed its snow amount, and every historical road must be traversed
exactly as many times as it has tons of snow.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_lines = [
        line for line in open(sys.argv[2]).read().splitlines() if line.split()
    ]
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, a, b = (int(x) for x in in_tokens[:4])
    roads = {}
    for i in range(m):
        x, y, w, t = (int(v) for v in in_tokens[4 + 4 * i:8 + 4 * i])
        roads[(x, y)] = (w, t)

    expected_p = int(ans_tokens[0])
    if not out_lines:
        reject("empty output")
    first = out_lines[0].split()
    if len(first) != 1:
        reject("first line must contain only the day count")
    try:
        p = int(first[0])
    except ValueError:
        reject(f"day count is not an integer: '{first[0]}'")
    if p != expected_p:
        reject(f"day count is {expected_p}, got {p}")

    if expected_p == 0:
        # No valid sequence of routes exists (the historical demand cannot be
        # met). The output is just the single "0"; nothing more to validate.
        if len(out_lines) != 1:
            reject("trailing output after a zero day count")
        sys.exit(0)

    if len(out_lines) != 1 + p:
        reject(f"expected {p} route lines, got {len(out_lines) - 1}")

    used = {}
    for idx, line in enumerate(out_lines[1:], 1):
        try:
            route = [int(v) for v in line.split()]
        except ValueError:
            reject(f"route {idx} contains a non-integer token")
        if len(route) < 2:
            reject(f"route {idx} has fewer than two junctions")
        if route[0] != a or route[-1] != b:
            reject(f"route {idx} does not go from {a} to {b}")
        if any(not (1 <= v <= n) for v in route):
            reject(f"route {idx} has a junction out of range")
        for u, v in zip(route, route[1:]):
            if (u, v) not in roads:
                reject(f"route {idx} uses nonexistent road {u}->{v}")
            used[(u, v)] = used.get((u, v), 0) + 1

    for (u, v), cnt in used.items():
        if cnt > roads[(u, v)][0]:
            reject(
                f"road {u}->{v} traversed {cnt} times but has only "
                f"{roads[(u, v)][0]} tons of snow"
            )

    for (u, v), (w, t) in roads.items():
        if t == 1 and used.get((u, v), 0) != w:
            reject(
                f"historical road {u}->{v} traversed "
                f"{used.get((u, v), 0)} times, needs exactly {w}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
