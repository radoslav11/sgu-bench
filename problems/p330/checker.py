"""Checker for p330: usage checker.py <in> <out> <ans>; exit 0 = accept.

Possibility is deterministic, so "Impossible" must match the reference.
A feasible answer is any sequence of at most 500 numbers starting at A,
ending at B, where each step adds a proper divisor d (1 < d < x, d | x)
of the current number x. The reference's own sequence is never compared.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    a, b = int(in_tokens[0]), int(in_tokens[1])
    ref_impossible = ans_tokens[0].lower() == "impossible"

    if not out_tokens:
        reject("empty output")

    if out_tokens[0].lower() == "impossible":
        if not ref_impossible:
            reject("said Impossible but a sequence exists")
        if len(out_tokens) > 1:
            reject("trailing output after 'Impossible'")
        sys.exit(0)

    if ref_impossible:
        reject("printed a sequence but the answer is Impossible")

    try:
        seq = [int(t) for t in out_tokens]
    except ValueError:
        reject(f"non-integer token in output: '{out_tokens[:5]}'")

    if len(seq) > 500:
        reject(f"sequence has {len(seq)} numbers, limit is 500")
    if seq[0] != a:
        reject(f"sequence starts with {seq[0]}, expected A={a}")
    if seq[-1] != b:
        reject(f"sequence ends with {seq[-1]}, expected B={b}")

    for i in range(len(seq) - 1):
        x, y = seq[i], seq[i + 1]
        d = y - x
        if not (1 < d < x):
            reject(f"step {x} -> {y}: d={d} not in (1, {x})")
        if x % d != 0:
            reject(f"step {x} -> {y}: {d} does not divide {x}")

    sys.exit(0)


if __name__ == "__main__":
    main()
