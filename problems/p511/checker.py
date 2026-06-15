"""Checker for p511: usage checker.py <in> <out> <ans>; exit 0 = accept.

For each test case the existence of a solution is deterministic, so whether
the contestant outputs -1 must agree with the reference. When the reference
reports -1 the contestant must also report -1. When a triple is expected the
contestant must supply some x, y, z with 1 <= x,y,z <= p-1 and
x^n + y^n == z^n (mod p); any such triple is accepted regardless of the
reference's own choice.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    ti = 0
    t = int(in_tokens[ti]); ti += 1
    cases = []
    for _ in range(t):
        n = int(in_tokens[ti]); p = int(in_tokens[ti + 1]); ti += 2
        cases.append((n, p))

    oi = 0  # index into out_tokens
    ai = 0  # index into ans_tokens

    for case_idx in range(t):
        n, p = cases[case_idx]

        # Parse the reference line: either "-1" or three integers.
        if ai >= len(ans_tokens):
            reject(f"reference missing answer for case {case_idx + 1}")
        if ans_tokens[ai] == "-1":
            ref_has = False
            ai += 1
        else:
            ref_has = True
            ai += 3

        # Parse the contestant line.
        if oi >= len(out_tokens):
            reject(f"output missing answer for case {case_idx + 1}")

        first = out_tokens[oi]
        if first == "-1":
            if ref_has:
                reject(f"case {case_idx + 1}: a solution exists but output is -1")
            oi += 1
            continue

        # Contestant claims a triple.
        if not ref_has:
            reject(f"case {case_idx + 1}: no solution exists but a triple was output")
        if oi + 3 > len(out_tokens):
            reject(f"case {case_idx + 1}: incomplete triple")
        try:
            x = int(out_tokens[oi]); y = int(out_tokens[oi + 1]); z = int(out_tokens[oi + 2])
        except ValueError:
            reject(f"case {case_idx + 1}: non-integer in triple")
        oi += 3

        for name, v in (("x", x), ("y", y), ("z", z)):
            if not (1 <= v <= p - 1):
                reject(f"case {case_idx + 1}: {name}={v} out of range [1, {p - 1}]")

        lhs = (pow(x, n, p) + pow(y, n, p)) % p
        rhs = pow(z, n, p) % p
        if lhs != rhs:
            reject(
                f"case {case_idx + 1}: x^n+y^n={lhs} != z^n={rhs} (mod {p})"
            )

    if oi != len(out_tokens):
        reject(f"trailing tokens in output after {t} cases")

    sys.exit(0)


if __name__ == "__main__":
    main()
