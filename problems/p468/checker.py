"""Checker for p468: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic, so the verdict line must agree with the
reference answer. Any valid open knight's tour is accepted: the grid
must be a permutation of 1..N^2 in which consecutive numbers are a
knight's move apart. The reference's own tour is never compared.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    solvable = ans_tokens and ans_tokens[0].lower() == "there"

    if not out_tokens:
        reject("empty output")

    if not solvable:
        text = " ".join(out_tokens).lower().rstrip(".")
        if text != "no solution":
            reject(f"expected 'No solution.', got '{' '.join(out_tokens)}'")
        sys.exit(0)

    header = [t.lower().rstrip(":") for t in out_tokens[:3]]
    if header != ["there", "is", "solution"]:
        reject("missing 'There is solution:' header")

    numbers = out_tokens[3:]
    if len(numbers) != n * n:
        reject(f"expected {n * n} numbers, got {len(numbers)}")

    pos = [None] * (n * n + 1)
    for idx, tok in enumerate(numbers):
        try:
            v = int(tok)
        except ValueError:
            reject(f"non-integer token '{tok}'")
        if not 1 <= v <= n * n:
            reject(f"value {v} out of range 1..{n * n}")
        if pos[v] is not None:
            reject(f"value {v} appears twice")
        pos[v] = (idx // n, idx % n)

    for v in range(1, n * n):
        dx = abs(pos[v][0] - pos[v + 1][0])
        dy = abs(pos[v][1] - pos[v + 1][1])
        if sorted((dx, dy)) != [1, 2]:
            reject(f"step {v} -> {v + 1} is not a knight move")

    sys.exit(0)


if __name__ == "__main__":
    main()
