"""Checker for p416: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any permutation of 1..n whose risk R (sum of squared differences along
the two rings plus across the spokes) is at least the reference's risk
is accepted; the exact arrangement is never compared.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def risk(n, a):
    total = 0
    for i in range(n):
        total += (a[i] - a[(i + 2) % n]) ** 2
    for i in range(n // 2):
        total += (a[2 * i] - a[2 * i + 1]) ** 2
    return total


def parse(tokens, n):
    if len(tokens) != n:
        reject(f"expected {n} numbers, got {len(tokens)}")
    try:
        a = [int(t) for t in tokens]
    except ValueError:
        reject("non-integer token in output")
    if sorted(a) != list(range(1, n + 1)):
        reject("output is not a permutation of 1..n")
    return a


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    a_out = parse(out_tokens, n)
    a_ans = [int(t) for t in ans_tokens]

    r_out = risk(n, a_out)
    r_ans = risk(n, a_ans)
    if r_out < r_ans:
        reject(f"risk {r_out} is below optimal {r_ans}")

    sys.exit(0)


if __name__ == "__main__":
    main()
