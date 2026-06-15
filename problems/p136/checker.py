"""Checker for p136: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO part is deterministic and must match the reference. A "YES"
must come with N vertices whose consecutive midpoints (cyclically)
reproduce the input midpoints within 1.5e-3; any such polygon is
accepted, the reference's own vertices are never compared against.
"""

import sys

EPS = 1.5e-3


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    mids = [float(t) for t in in_tokens[1:1 + 2 * n]]

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    if len(out_tokens) != 1 + 2 * n:
        reject(f"expected {2 * n} coordinates, got {len(out_tokens) - 1} tokens")
    try:
        coords = [float(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-numeric vertex coordinate")

    for i in range(n):
        x1, y1 = coords[2 * i], coords[2 * i + 1]
        j = (i + 1) % n
        x2, y2 = coords[2 * j], coords[2 * j + 1]
        mx, my = mids[2 * i], mids[2 * i + 1]
        if abs((x1 + x2) / 2 - mx) > EPS or abs((y1 + y2) / 2 - my) > EPS:
            reject(
                f"edge {i + 1}: midpoint ({(x1 + x2) / 2}, {(y1 + y2) / 2})"
                f" differs from ({mx}, {my})"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
