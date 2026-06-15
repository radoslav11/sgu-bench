"""Checker for p125: usage checker.py <in> <out> <ans>; exit 0 = accept.

Existence is deterministic, so "NO SOLUTION" must match the reference.
A claimed matrix A is validated directly against the input matrix B:
every value must be in 0..9 and each cell must have exactly B[i][j]
strictly larger orthogonal neighbors. Any valid A is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_text = open(sys.argv[3]).read()

    ref_no = "NO" in ans_text.upper().split()
    out_upper = out_text.upper().split()
    got_no = out_upper[:2] == ["NO", "SOLUTION"]

    if ref_no:
        if not got_no:
            reject("a solution does not exist, expected NO SOLUTION")
        sys.exit(0)
    if got_no:
        reject("a solution exists, got NO SOLUTION")

    n = int(in_tokens[0])
    b = [int(x) for x in in_tokens[1:1 + n * n]]

    out_tokens = out_text.split()
    if len(out_tokens) != n * n:
        reject(f"expected {n * n} numbers, got {len(out_tokens)} tokens")
    try:
        a = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer token in output matrix")
    if any(not (0 <= v <= 9) for v in a):
        reject("army size out of range 0..9")

    for i in range(n):
        for j in range(n):
            count = 0
            for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                x, y = i + di, j + dj
                if 0 <= x < n and 0 <= y < n:
                    if a[x * n + y] > a[i * n + j]:
                        count += 1
            if count != b[i * n + j]:
                reject(
                    f"cell ({i + 1},{j + 1}): B is {b[i * n + j]}, "
                    f"matrix gives {count}"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
