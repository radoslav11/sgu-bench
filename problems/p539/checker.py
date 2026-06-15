"""Checker for p539: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum number of steps k is deterministic, so the first output token
must equal the reference's k. The k multiswaps themselves are free: each
must use distinct in-range indices with no index repeated within a step,
and applying every step in order must sort the array. Any valid plan with
the correct k is accepted; the reference's plan is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    a = [int(x) for x in in_tokens[1:1 + n]]

    expected_k = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if k != expected_k:
        reject(f"minimum steps is {expected_k}, got {k}")

    idx = 1
    arr = list(a)
    for step in range(k):
        if idx >= len(out_tokens):
            reject(f"step {step + 1}: missing pair count")
        p = int(out_tokens[idx])
        idx += 1
        if p <= 0:
            reject(f"step {step + 1}: pair count must be > 0, got {p}")
        if idx + 2 * p > len(out_tokens):
            reject(f"step {step + 1}: not enough indices for {p} pairs")

        used = set()
        pairs = []
        for _ in range(p):
            i = int(out_tokens[idx])
            j = int(out_tokens[idx + 1])
            idx += 2
            if not (1 <= i <= n) or not (1 <= j <= n):
                reject(f"step {step + 1}: index out of range ({i}, {j})")
            if i == j:
                reject(f"step {step + 1}: pair uses equal indices ({i}, {j})")
            if i in used or j in used:
                reject(f"step {step + 1}: index reused within step")
            used.add(i)
            used.add(j)
            pairs.append((i - 1, j - 1))

        for i, j in pairs:
            arr[i], arr[j] = arr[j], arr[i]

    if idx != len(out_tokens):
        reject("trailing output after the declared steps")

    if arr != sorted(a):
        reject("array is not sorted after applying all steps")

    sys.exit(0)


if __name__ == "__main__":
    main()
