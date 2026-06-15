"""Checker for p199: usage checker.py <in> <out> <ans>; exit 0 = accept.

The invited-set size is deterministic and must match the reference. The
set itself is a certificate: indices must be distinct, in range, and
pairwise non-hating — i.e. sortable so that both strength and beauty are
strictly increasing. Any such set of the right size is accepted.
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
    s = [0] * (n + 1)
    b = [0] * (n + 1)
    for i in range(n):
        s[i + 1] = int(in_tokens[1 + 2 * i])
        b[i + 1] = int(in_tokens[2 + 2 * i])

    expected_k = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if k != expected_k:
        reject(f"optimal size is {expected_k}, got {k}")

    ids = out_tokens[1:]
    if len(ids) != k:
        reject(f"declared {k} members, listed {len(ids)}")
    try:
        ids = [int(x) for x in ids]
    except ValueError:
        reject("non-integer member index")
    if len(set(ids)) != k:
        reject("duplicate member indices")
    if any(not (1 <= i <= n) for i in ids):
        reject("member index out of range")

    chosen = sorted((s[i], b[i]) for i in ids)
    for (s1, b1), (s2, b2) in zip(chosen, chosen[1:]):
        if s1 >= s2 or b1 >= b2:
            reject(
                f"members with (S,B)=({s1},{b1}) and ({s2},{b2}) hate"
                " each other"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
