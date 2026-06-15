"""Checker for p388: usage checker.py <in> <out> <ans>; exit 0 = accept.

The marriage count k is deterministic and must match the reference. The
two matchings are certificates: Juan's k pairs must come from Juan's
input list (either orientation), Rosa's from Rosa's, each matching must
use 2k distinct actors, and both matchings must cover the same troup.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def read_matching(tokens, pos, k, allowed, n, who):
    pairs = []
    if len(tokens) < pos + 2 * k:
        reject(f"{who}: expected {k} pairs, output too short")
    for i in range(k):
        try:
            a = int(tokens[pos + 2 * i])
            b = int(tokens[pos + 2 * i + 1])
        except ValueError:
            reject(f"{who}: pair {i + 1} is not two integers")
        if not (1 <= a <= n and 1 <= b <= n):
            reject(f"{who}: actor out of range in pair {i + 1}")
        if (min(a, b), max(a, b)) not in allowed:
            reject(f"{who}: pair ({a}, {b}) is not in the input list")
        pairs.append((a, b))
    used = [x for p in pairs for x in p]
    if len(set(used)) != 2 * k:
        reject(f"{who}: an actor appears in more than one marriage")
    return set(used)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m1, m2 = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    flat = [int(x) for x in in_tokens[3:3 + 2 * (m1 + m2)]]
    juan = set()
    for i in range(m1):
        a, b = flat[2 * i], flat[2 * i + 1]
        juan.add((min(a, b), max(a, b)))
    rosa = set()
    for i in range(m1, m1 + m2):
        a, b = flat[2 * i], flat[2 * i + 1]
        rosa.add((min(a, b), max(a, b)))

    k_ref = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be k, got '{out_tokens[0]}'")
    if k != k_ref:
        reject(f"maximal number of marriages is {k_ref}, got {k}")

    troup_j = read_matching(out_tokens, 1, k, juan, n, "Juan")
    troup_r = read_matching(out_tokens, 1 + 2 * k, k, rosa, n, "Rosa")
    if troup_j != troup_r:
        reject("Juan's and Rosa's matchings cover different actor sets")
    if len(out_tokens) > 1 + 4 * k:
        reject("trailing output after the two matchings")

    sys.exit(0)


if __name__ == "__main__":
    main()
