"""Checker for p462 (Electrician): usage checker.py <in> <out> <ans>.

The maximal total cost is deterministic and must equal the reference's
value on the first line. The second line is a soldering order: a
permutation of 1..n. We validate it is a permutation and recompute the
final scheme's total cost from it.

A wire survives the soldering process iff it belongs to the maximum
spanning forest under the key (reliability, position-in-order): higher
reliability always wins, and on equal reliability the wire soldered later
survives (the earlier one burns). The recomputed cost must equal the
reference value, so any valid order is accepted but a cheaper one is not.
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
    wires = []
    idx = 1
    for _ in range(n):
        a = int(in_tokens[idx]); b = int(in_tokens[idx + 1])
        r = int(in_tokens[idx + 2]); p = int(in_tokens[idx + 3])
        idx += 4
        wires.append((a, b, r, p))

    if not out_tokens:
        reject("empty output")
    if not ans_tokens:
        reject("empty reference answer")

    try:
        expected_cost = int(ans_tokens[0])
    except ValueError:
        reject("reference first token is not an integer")

    try:
        out_cost = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the total cost, got '{out_tokens[0]}'")

    order_tokens = out_tokens[1:]
    if len(order_tokens) != n:
        reject(f"order must list exactly {n} indices, got {len(order_tokens)}")
    try:
        order = [int(x) for x in order_tokens]
    except ValueError:
        reject("non-integer in order")

    if sorted(order) != list(range(1, n + 1)):
        reject("order is not a permutation of 1..n")

    # position[w] = soldering position of wire w (0-based, in given order).
    position = [0] * (n + 1)
    for pos, w in enumerate(order):
        position[w] = pos

    # Coordinate compress node ids.
    coords = sorted({c for (a, b, r, p) in wires for c in (a, b)})
    comp = {c: i for i, c in enumerate(coords)}
    m = len(coords)

    par = list(range(m))

    def root(x):
        while par[x] != x:
            par[x] = par[par[x]]
            x = par[x]
        return x

    # Maximum spanning forest by key (reliability, position): process edges
    # in decreasing key order, take an edge if its endpoints differ.
    indexed = sorted(
        range(n),
        key=lambda i: (wires[i][2], position[i + 1]),
        reverse=True,
    )

    real_cost = 0
    for i in indexed:
        a, b, r, p = wires[i]
        ra, rb = root(comp[a]), root(comp[b])
        if ra != rb:
            par[ra] = rb
            real_cost += p

    if real_cost != expected_cost:
        reject(
            f"order yields total cost {real_cost}, "
            f"but the maximum is {expected_cost}"
        )

    if out_cost != expected_cost:
        reject(
            f"printed cost {out_cost} does not match the achievable "
            f"cost {real_cost}"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
