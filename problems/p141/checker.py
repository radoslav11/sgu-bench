"""Checker for p141: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict is deterministic and must match the reference. A YES
needs four non-negative integers P1 N1 P2 N2 with
P1*x1 - N1*x1 + P2*x2 - N2*x2 = P and P1+N1+P2+N2 = K; any such
quadruple is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    x1, x2, p, k = (int(t) for t in in_tokens[:4])

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

    if len(out_tokens) != 5:
        reject(f"expected 4 integers after YES, got {len(out_tokens) - 1}")
    try:
        p1, n1, p2, n2 = (int(t) for t in out_tokens[1:5])
    except ValueError:
        reject("non-integer token after YES")
    if min(p1, n1, p2, n2) < 0:
        reject("counts must be non-negative")
    if p1 + n1 + p2 + n2 != k:
        reject(f"total jumps {p1 + n1 + p2 + n2}, expected {k}")
    reached = (p1 - n1) * x1 + (p2 - n2) * x2
    if reached != p:
        reject(f"landed on {reached}, expected {p}")

    sys.exit(0)


if __name__ == "__main__":
    main()
