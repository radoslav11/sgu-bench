"""Checker for p205: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal deviation is deterministic and must match the reference. The
index sequence is a certificate: simulating it from set 0 (next set is
k & (m-1)) must reproduce exactly the claimed minimal deviation.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    n = int(in_tokens[pos]); pos += 1
    xs = [int(t) for t in in_tokens[pos:pos + n]]; pos += n
    m = int(in_tokens[pos]); s = int(in_tokens[pos + 1]); pos += 2
    L = []
    for _ in range(m):
        L.append([int(t) for t in in_tokens[pos:pos + s]])
        pos += s

    expected = int(ans_tokens[0])

    if len(out_tokens) != n + 1:
        reject(f"expected {n + 1} tokens, got {len(out_tokens)}")
    try:
        claimed = int(out_tokens[0])
        ks = [int(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-integer token in output")
    if claimed != expected:
        reject(f"deviation is {expected}, got {claimed}")

    total = 0
    cur = 0
    for j, k in enumerate(ks):
        if not (0 <= k < s):
            reject(f"index {k} at position {j + 1} out of range [0, {s - 1}]")
        total += abs(xs[j] - L[cur][k])
        cur = k & (m - 1)

    if total != expected:
        reject(f"certificate deviation {total} != minimal {expected}")

    sys.exit(0)


if __name__ == "__main__":
    main()
