"""Checker for p366: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any K distinct opponents (listed ascending) are accepted as long as the
printed A, B match the chosen set and the pair achieves the same optimal
|A - B| and the same maximal A + B as the reference. The reference's own
selection (and even its exact A, B split) is never required: a mirrored
solution with A and B swapped is equally optimal.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, k = int(in_tokens[0]), int(in_tokens[1])
    vals = [
        (int(in_tokens[2 + 2 * i]), int(in_tokens[3 + 2 * i]))
        for i in range(n)
    ]

    ref_a, ref_b = int(ans_tokens[0]), int(ans_tokens[1])

    if len(out_tokens) != 2 + k:
        reject(f"expected {2 + k} numbers (A B + {k} indices), got {len(out_tokens)}")
    try:
        nums = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    a_out, b_out = nums[0], nums[1]
    idxs = nums[2:]

    if len(set(idxs)) != k:
        reject("selected opponents are not distinct")
    if any(not (1 <= i <= n) for i in idxs):
        reject("opponent index out of range")
    if idxs != sorted(idxs):
        reject("indices not in ascending order")

    a = sum(vals[i - 1][0] for i in idxs)
    b = sum(vals[i - 1][1] for i in idxs)
    if (a, b) != (a_out, b_out):
        reject(f"printed A B = {a_out} {b_out}, but selection gives {a} {b}")

    if abs(a - b) != abs(ref_a - ref_b):
        reject(f"|A-B| = {abs(a - b)}, optimum is {abs(ref_a - ref_b)}")
    if a + b != ref_a + ref_b:
        reject(f"A+B = {a + b}, optimum is {ref_a + ref_b}")

    sys.exit(0)


if __name__ == "__main__":
    main()
