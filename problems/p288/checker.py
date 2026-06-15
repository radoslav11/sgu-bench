"""Checker for p288: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal tour count T is unique and must match the reference. The
schedule matrix is a certificate: NxN, zero diagonal, symmetric, every
off-diagonal entry in 1..T, and no row repeats a tour number (a player
never plays twice in one tour). Any matrix satisfying that is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    expected_t = int(open(sys.argv[3]).read().split()[0])

    if not out_tokens:
        reject("empty output")
    try:
        values = [int(tok) for tok in out_tokens]
    except ValueError:
        reject("non-integer token in output")
    if len(values) != 1 + n * n:
        reject(f"expected {1 + n * n} numbers, got {len(values)}")

    t = values[0]
    if t != expected_t:
        reject(f"minimal number of tours is {expected_t}, got {t}")

    matrix = values[1:]
    for i in range(n):
        row = matrix[i * n:(i + 1) * n]
        if row[i] != 0:
            reject(f"diagonal entry ({i + 1},{i + 1}) must be 0")
        seen = set()
        for j in range(n):
            if j == i:
                continue
            v = row[j]
            if not (1 <= v <= t):
                reject(f"entry ({i + 1},{j + 1}) = {v} out of range 1..{t}")
            if v in seen:
                reject(f"player {i + 1} plays twice in tour {v}")
            seen.add(v)
            if matrix[j * n + i] != v:
                reject(f"matrix not symmetric at ({i + 1},{j + 1})")

    sys.exit(0)


if __name__ == "__main__":
    main()
