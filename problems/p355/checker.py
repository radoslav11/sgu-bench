"""Checker for p355: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal number of colors M is deterministic and must match the
reference. The coloring itself is a certificate: colors must lie in
[1, M] and whenever B properly divides A the colors of A and B must
differ. Any valid coloring is accepted; the reference's own coloring is
never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected_m = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        m = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if m != expected_m:
        reject(f"minimal number of colors is {expected_m}, got {m}")

    if len(out_tokens) != 1 + n:
        reject(f"expected {n} colors, got {len(out_tokens) - 1} tokens")
    try:
        color = [0] + [int(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-integer color token")
    if any(not (1 <= c <= m) for c in color[1:]):
        reject(f"color out of range [1, {m}]")

    # color[2*b::b] lists the colors of all proper multiples of b, so a
    # C-level "in" scan per b checks every divisor pair in O(N log N).
    for b in range(1, n // 2 + 1):
        if color[b] in color[2 * b::b]:
            a = next(
                a for a in range(2 * b, n + 1, b) if color[a] == color[b]
            )
            reject(f"{b} divides {a} but both have color {color[b]}")

    sys.exit(0)


if __name__ == "__main__":
    main()
