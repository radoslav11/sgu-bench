"""Checker for p381: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict and the minimal number of transformations must match
the reference. The list of flipped vertices is a certificate: after
negating all signs at the listed vertices, every edge must have its two
directions multiplying to -1 (one tail, one head). Any vertex list of
the minimal length that achieves this is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

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

    ref_k = int(ans_tokens[1])
    if len(out_tokens) < 2:
        reject("missing transformation count")
    try:
        k = int(out_tokens[1])
    except ValueError:
        reject(f"transformation count is not an integer: '{out_tokens[1]}'")
    if k != ref_k:
        reject(f"minimal transformation count is {ref_k}, got {k}")
    if len(out_tokens) != 2 + k:
        reject(f"expected {k} vertex indices, got {len(out_tokens) - 2}")

    n, m = int(in_tokens[0]), int(in_tokens[1])
    flip = [0] * (n + 1)
    for tok in out_tokens[2:]:
        try:
            v = int(tok)
        except ValueError:
            reject(f"vertex index is not an integer: '{tok}'")
        if not (1 <= v <= n):
            reject(f"vertex index {v} out of range 1..{n}")
        flip[v] ^= 1

    pos = 2
    for i in range(m):
        a = int(in_tokens[pos])
        b = int(in_tokens[pos + 1])
        d1 = int(in_tokens[pos + 2])
        d2 = int(in_tokens[pos + 3])
        pos += 4
        if flip[a]:
            d1 = -d1
        if flip[b]:
            d2 = -d2
        if d1 * d2 != -1:
            reject(f"edge {i + 1} ({a},{b}) is not directed after flips")

    sys.exit(0)


if __name__ == "__main__":
    main()
