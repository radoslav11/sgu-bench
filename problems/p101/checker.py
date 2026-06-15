"""Checker for p101: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic, so "No solution" must match the reference.
A claimed arrangement is a certificate: every piece index 1..N exactly
once with '+'/'-', and consecutive pieces touch with equal digits. Any
valid arrangement is accepted; the reference's order is never compared.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_text = open(sys.argv[3]).read()

    n = int(in_tokens[0])
    pieces = [
        (int(in_tokens[1 + 2 * i]), int(in_tokens[2 + 2 * i]))
        for i in range(n)
    ]

    solvable = "No solution" not in ans_text
    out_says_no = " ".join(t.lower() for t in out_tokens) == "no solution"

    if not solvable:
        if not out_says_no:
            reject("answer is 'No solution', got an arrangement")
        sys.exit(0)
    if out_says_no:
        reject("a valid arrangement exists, got 'No solution'")

    if len(out_tokens) != 2 * n:
        reject(f"expected {n} '<index> <sign>' lines, got {len(out_tokens)} tokens")

    seen = set()
    prev_right = None
    for i in range(n):
        idx_tok, sign = out_tokens[2 * i], out_tokens[2 * i + 1]
        try:
            idx = int(idx_tok)
        except ValueError:
            reject(f"bad piece index '{idx_tok}'")
        if not (1 <= idx <= n):
            reject(f"piece index {idx} out of range")
        if idx in seen:
            reject(f"piece {idx} used twice")
        seen.add(idx)
        if sign not in ("+", "-"):
            reject(f"bad sign '{sign}'")
        a, b = pieces[idx - 1]
        if sign == "-":
            a, b = b, a
        if prev_right is not None and prev_right != a:
            reject(f"position {i + 1}: left digit {a} != previous right {prev_right}")
        prev_right = b

    sys.exit(0)


if __name__ == "__main__":
    main()
