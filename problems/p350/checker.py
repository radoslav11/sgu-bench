"""Checker for p350: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any set A of N numbers in [0, 2^31 - 1] whose multiset of pairwise XORs
equals the input multiset B is accepted; the reference's own A is never
compared against.
"""

import math
import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    m = int(in_tokens[0])
    b = sorted(int(x) for x in in_tokens[1 : 1 + m])
    n = int((1 + math.isqrt(1 + 8 * m)) // 2)

    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)} tokens")

    try:
        a = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    for v in a:
        if not (0 <= v <= (1 << 31) - 1):
            reject(f"value {v} out of [0, 2^31 - 1]")

    got = sorted(a[i] ^ a[j] for i in range(n) for j in range(i + 1, n))
    if got != b:
        reject("pairwise XOR multiset does not match B")

    sys.exit(0)


if __name__ == "__main__":
    main()
