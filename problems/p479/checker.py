"""Checker for p479: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic, so "No solution" must match the reference.
Otherwise the output is n*m cell coordinates, each cell exactly once;
the machine is simulated (planting a pumpkin also adds one to every
adjacent cell that already holds at least one) and the resulting grid
must equal the input plan.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    target = [int(x) for x in in_tokens[2:2 + n * m]]

    no_solution = ans_tokens[0].lower() == "no"

    if not out_tokens:
        reject("empty output")
    if out_tokens[0].lower() == "no":
        if " ".join(out_tokens).lower() != "no solution":
            reject("malformed output starting with 'no'")
        if not no_solution:
            reject("said 'No solution' but a solution exists")
        sys.exit(0)
    if no_solution:
        reject("there is no solution, but output gives operations")

    if len(out_tokens) != 2 * n * m:
        reject(f"expected {2 * n * m} integers, got {len(out_tokens)}")
    try:
        vals = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer tokens in operation list")

    grid = [0] * (n * m)
    used = [False] * (n * m)
    for k in range(n * m):
        r, c = vals[2 * k], vals[2 * k + 1]
        if not (1 <= r <= n) or not (1 <= c <= m):
            reject(f"operation {k + 1}: cell ({r}, {c}) out of range")
        idx = (r - 1) * m + (c - 1)
        if used[idx]:
            reject(f"operation {k + 1}: cell ({r}, {c}) specified twice")
        used[idx] = True
        for dr, dc in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            nr, nc = r - 1 + dr, c - 1 + dc
            if 0 <= nr < n and 0 <= nc < m and grid[nr * m + nc] >= 1:
                grid[nr * m + nc] += 1
        grid[idx] += 1

    if grid != target:
        for i in range(n * m):
            if grid[i] != target[i]:
                reject(
                    f"cell ({i // m + 1}, {i % m + 1}) ends with {grid[i]} "
                    f"pumpkins, plan wants {target[i]}"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
