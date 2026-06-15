"""Checker for p482 (Impudent Thief): usage checker.py <in> <out> <ans>.

The maximum stolen total height s is deterministic and must equal the
reference's first number. The stolen subset is a certificate: it must be
distinct valid board indices, its heights must sum to s, and the fence
left from the boards NOT stolen (kept in original order) must have a
perimeter of at least ceil(P/2), where P is the original perimeter.

Perimeter of a fence with kept heights g[0..m-1] (joining unit-width
rectangles) is 2 + sum_i (2 + |g[i]-g[i+1]|) + g[0] + g[m-1].
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def perimeter(heights):
    if not heights:
        return 0
    p = 2
    for i in range(len(heights) - 1):
        p += 2 + abs(heights[i] - heights[i + 1])
    p += heights[0] + heights[-1]
    return p


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    h = [int(x) for x in in_tokens[1:1 + n]]
    if len(h) != n:
        reject("input malformed")

    orig_p = perimeter(h)
    target = (orig_p + 1) // 2

    if not ans_tokens:
        reject("empty reference answer")
    expected_s = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        out_s = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be integer s, got '{out_tokens[0]}'")

    if len(out_tokens) < 2:
        reject("missing count k")
    try:
        k = int(out_tokens[1])
    except ValueError:
        reject(f"second token must be integer k, got '{out_tokens[1]}'")

    idx_tokens = out_tokens[2:]
    if len(idx_tokens) != k:
        reject(f"declared k={k} stolen boards but listed {len(idx_tokens)}")

    try:
        stolen = [int(x) for x in idx_tokens]
    except ValueError:
        reject("non-integer board index")

    if len(set(stolen)) != len(stolen):
        reject("duplicate board index in stolen list")
    for b in stolen:
        if not (1 <= b <= n):
            reject(f"board index {b} out of range 1..{n}")

    stolen_set = set(stolen)
    stolen_sum = sum(h[b - 1] for b in stolen)
    if stolen_sum != out_s:
        reject(f"printed s={out_s} but listed boards sum to {stolen_sum}")

    if out_s != expected_s:
        reject(f"stolen total {out_s} is not the maximum {expected_s}")

    kept = [h[i] for i in range(n) if (i + 1) not in stolen_set]
    new_p = perimeter(kept)
    if new_p < target:
        reject(
            f"remaining fence perimeter {new_p} < required {target} "
            f"(half of {orig_p})"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
