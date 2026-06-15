"""Checker for p164: usage checker.py <in> <out> <ans>; exit 0 = accept.

A solution always exists (one parity class of companies always yields
diameter <= 3), so "-1" is accepted only if the reference also printed
-1. Otherwise the submission must list at most (M+1) div 2 distinct
company ids in [1, M] whose flights connect every pair of the N cities
by a path of at most 3 free edges. Any such set is accepted; the
reference's own set is never compared against.
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
    m = int(in_tokens[1])
    owner = [
        [int(in_tokens[2 + i * n + j]) for j in range(n)] for i in range(n)
    ]

    if not out_tokens:
        reject("empty output")
    try:
        count = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")

    ref_has_solution = ans_tokens[0] != "-1"
    if count == -1:
        if ref_has_solution:
            reject("claimed no solution but reference found one")
        if len(out_tokens) > 1:
            reject("trailing output after -1")
        sys.exit(0)
    if not ref_has_solution:
        reject("reference printed -1 but submission claims a solution")

    if count < 0 or count > (m + 1) // 2:
        reject(f"bought {count} parts, limit is {(m + 1) // 2}")
    bought_list = []
    for tok in out_tokens[1:]:
        try:
            bought_list.append(int(tok))
        except ValueError:
            reject(f"non-integer company id '{tok}'")
    if len(bought_list) != count:
        reject(f"declared {count} parts, listed {len(bought_list)}")
    if len(set(bought_list)) != count:
        reject("duplicate company ids")
    if any(not (1 <= p <= m) for p in bought_list):
        reject("company id out of range")

    bought = set(bought_list)
    adj = [0] * n
    for i in range(n):
        for j in range(n):
            if i != j and owner[i][j] in bought:
                adj[i] |= 1 << j

    full = (1 << n) - 1
    for s in range(n):
        reach = 1 << s
        for _ in range(3):
            nxt = reach
            r = reach
            while r:
                v = (r & -r).bit_length() - 1
                r &= r - 1
                nxt |= adj[v]
            if nxt == reach:
                break
            reach = nxt
        if reach != full:
            far = (full ^ reach).bit_length() - 1
            reject(f"city {far + 1} is farther than 3 free flights from city {s + 1}")

    sys.exit(0)


if __name__ == "__main__":
    main()
