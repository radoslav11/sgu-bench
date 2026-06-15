"""Checker for p152: usage checker.py <in> <out> <ans>; exit 0 = accept.

The output must be N integers summing to exactly 100, where part i is
Ai*100/sum(A) rounded either down or up (exact value mandatory when the
real percentage is already an integer). Generated tests always have a
positive vote total, so a solution always exists and "No solution" is
rejected.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n = int(in_tokens[0])
    a = [int(x) for x in in_tokens[1:1 + n]]
    total = sum(a)

    if any(not tok.lstrip("-").isdigit() for tok in out_tokens):
        reject("output is not a sequence of integers (a solution exists)")
    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")

    parts = [int(x) for x in out_tokens]
    if sum(parts) != 100:
        reject(f"parts sum to {sum(parts)}, must be exactly 100")

    for i, (votes, p) in enumerate(zip(a, parts)):
        lo = (votes * 100) // total
        hi = lo if (votes * 100) % total == 0 else lo + 1
        if p != lo and p != hi:
            reject(
                f"part {i + 1} is {p}, but {votes}*100/{total} rounds "
                f"to {lo} or {hi}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
