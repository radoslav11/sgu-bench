"""Checker for p524: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum total shift t is deterministic and must match the reference
value (float tolerance). The new coordinates are a certificate: any optimal
arrangement is accepted, so they are validated against the input -- they
must be strictly increasing, form an arithmetic progression (equal adjacent
spacing), and their own total shift must equal the reported t.
"""

import sys

EPS = 1e-4


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    x = [int(v) for v in in_tokens[1:1 + n]]

    if len(out_tokens) < 1 + n:
        reject(f"expected {1 + n} numbers, got {len(out_tokens)}")

    try:
        t = float(out_tokens[0])
        y = [float(v) for v in out_tokens[1:1 + n]]
    except ValueError:
        reject("non-numeric output")

    ref_t = float(ans_tokens[0])

    tol = EPS * max(1.0, abs(ref_t))
    if abs(t - ref_t) > tol:
        reject(f"t={t} differs from reference {ref_t}")

    # Strictly increasing.
    for i in range(1, n):
        if not (y[i] > y[i - 1] + 1e-9):
            reject(f"coordinates not strictly increasing at index {i}")

    # Arithmetic progression: equal spacing.
    d = (y[-1] - y[0]) / (n - 1)
    for i in range(1, n):
        spacing = y[i] - y[i - 1]
        if abs(spacing - d) > 1e-4:
            reject(f"spacing {spacing} != {d} at index {i}")

    # The certificate's own total shift must equal the reported t.
    cost = sum(abs(x[i] - y[i]) for i in range(n))
    ctol = EPS * max(1.0, abs(ref_t))
    if abs(cost - ref_t) > max(ctol, 1e-3):
        reject(f"certificate shift {cost} differs from reference {ref_t}")

    sys.exit(0)


if __name__ == "__main__":
    main()
