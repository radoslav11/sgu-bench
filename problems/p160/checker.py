"""Checker for p160: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximal score is deterministic and must match the reference. The
lever subset is "output any", so it is validated against the input:
distinct indices in ascending order whose product modulo M equals the
claimed (optimal) score. An empty subset is fine when the score is 1.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    a = [int(x) for x in in_tokens[2:2 + n]]

    best = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        score = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the score, got '{out_tokens[0]}'")
    if score != best:
        reject(f"maximal score is {best}, claimed {score}")

    try:
        levers = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("non-integer lever index")
    if any(not (1 <= x <= n) for x in levers):
        reject("lever index out of range")
    if any(levers[i] >= levers[i + 1] for i in range(len(levers) - 1)):
        reject("lever indices must be distinct and in ascending order")

    prod = 1
    for x in levers:
        prod = (prod * a[x - 1]) % m
    if prod != score:
        reject(f"selected levers give {prod}, claimed {score}")

    sys.exit(0)


if __name__ == "__main__":
    main()
