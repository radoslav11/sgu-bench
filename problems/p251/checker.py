"""Checker for p251: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict must match the reference. On YES the output must give
N coordinate pairs such that for every triple of points the triangle area
equals the sum of their special numbers; tolerance 0.02 absorbs the
4-decimal printing precision. Any valid arrangement is accepted.
"""

import sys
from itertools import combinations

EPS = 0.02


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    r = [int(x) for x in in_tokens[1 : 1 + n]]

    if not out_tokens:
        reject("empty output")
    expected = ans_tokens[0].upper()
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is {expected}, got {got}")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after NO")
        sys.exit(0)

    coords = out_tokens[1:]
    if len(coords) != 2 * n:
        reject(f"expected {2 * n} coordinates, got {len(coords)}")
    try:
        pts = [(float(coords[2 * i]), float(coords[2 * i + 1])) for i in range(n)]
    except ValueError:
        reject("non-numeric coordinate")

    for i, j, k in combinations(range(n), 3):
        (xi, yi), (xj, yj), (xk, yk) = pts[i], pts[j], pts[k]
        area = abs((xj - xi) * (yk - yi) - (xk - xi) * (yj - yi)) / 2.0
        want = r[i] + r[j] + r[k]
        if abs(area - want) > EPS:
            reject(
                f"triangle ({i + 1},{j + 1},{k + 1}) has area {area:.4f}, "
                f"expected {want}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
