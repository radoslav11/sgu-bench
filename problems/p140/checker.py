"""Checker for p140: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict is deterministic and must match the reference. A YES
must be followed by N non-negative integers below 2e9 whose dot product
with A is congruent to B modulo P; any such sequence is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, p, b = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    a = [int(x) for x in in_tokens[3 : 3 + n]]

    if not out_tokens:
        reject("empty output")
    expected = ans_tokens[0].upper()
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    if len(out_tokens) != 1 + n:
        reject(f"expected {n} integers after YES, got {len(out_tokens) - 1}")
    try:
        x = [int(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-integer token in X")
    if any(xi < 0 or xi >= 2000000000 for xi in x):
        reject("X element out of [0, 2e9) range")

    total = sum(ai * xi for ai, xi in zip(a, x))
    if total % p != b % p:
        reject(f"A*X mod P = {total % p}, expected {b % p}")

    sys.exit(0)


if __name__ == "__main__":
    main()
