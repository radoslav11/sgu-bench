"""Checker for p354: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic: if the reference printed 0, the output
must be exactly 0. Otherwise the output must be ANY n x n permutation of
1..n^2 whose top/left count matrices equal the input; the reference's
own matrix is never compared against.
"""

import sys
from bisect import bisect_right, insort


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    nn = n * n
    top = [int(x) for x in in_tokens[1 : 1 + nn]]
    left = [int(x) for x in in_tokens[1 + nn : 1 + 2 * nn]]

    no_solution = len(ans_tokens) == 1 and ans_tokens[0] == "0"

    if not out_tokens:
        reject("empty output")

    if no_solution:
        if len(out_tokens) == 1 and out_tokens[0] == "0":
            sys.exit(0)
        reject("no solution exists, expected single 0")

    if len(out_tokens) == 1 and out_tokens[0] == "0":
        reject("claimed no solution, but a solution exists")
    if len(out_tokens) != nn:
        reject(f"expected {nn} numbers, got {len(out_tokens)} tokens")

    try:
        a = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    if sorted(a) != list(range(1, nn + 1)):
        reject("output is not a permutation of 1..n^2")

    for i in range(n):
        seen = []
        for j in range(n):
            v = a[i * n + j]
            got = len(seen) - bisect_right(seen, v)
            if got != left[i * n + j]:
                reject(f"left[{i + 1}][{j + 1}] is {got}, expected {left[i * n + j]}")
            insort(seen, v)

    for j in range(n):
        seen = []
        for i in range(n):
            v = a[i * n + j]
            got = len(seen) - bisect_right(seen, v)
            if got != top[i * n + j]:
                reject(f"top[{i + 1}][{j + 1}] is {got}, expected {top[i * n + j]}")
            insort(seen, v)

    sys.exit(0)


if __name__ == "__main__":
    main()
