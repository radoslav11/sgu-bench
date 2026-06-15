"""Checker for p207: usage checker.py <in> <out> <ans>; exit 0 = accept.

K must be N non-negative integers summing to M. The total unfairness
sum |Xi/Y - Ki/M| = sum |Xi*M - Ki*Y| / (Y*M) is compared exactly via the
integer numerator against the reference's split; any split reaching the
same minimal unfairness is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, y = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    xs = [int(t) for t in in_tokens[3:3 + n]]

    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")
    try:
        ks = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")
    if any(k < 0 for k in ks):
        reject("negative coin count")
    if sum(ks) != m:
        reject(f"coins sum to {sum(ks)}, must be {m}")

    ref_ks = [int(t) for t in ans_tokens]
    ref_unfairness = sum(abs(xs[i] * m - ref_ks[i] * y) for i in range(n))
    got_unfairness = sum(abs(xs[i] * m - ks[i] * y) for i in range(n))
    if got_unfairness != ref_unfairness:
        reject(
            f"unfairness numerator {got_unfairness} != optimal "
            f"{ref_unfairness} (denominator Y*M)"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
