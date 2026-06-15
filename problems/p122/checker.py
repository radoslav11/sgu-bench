"""Checker for p122: usage checker.py <in> <out> <ans>; exit 0 = accept.

Every generated test satisfies the degree condition (minimum degree at
least floor((N+1)/2)), so by Dirac's theorem a Hamiltonian cycle always
exists and "No solution" is never a correct output. The cycle is a
certificate validated against the input: N+1 numbers starting and
ending at 1, consecutive entries friends, every man visited exactly
once. Any valid cycle is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_lines = open(sys.argv[1]).read().splitlines()
    out_text = open(sys.argv[2]).read()

    n = int(in_lines[0].split()[0])
    friends = [set() for _ in range(n + 1)]
    for i in range(1, n + 1):
        for tok in in_lines[i].split():
            friends[i].add(int(tok))

    if "no solution" in out_text.lower():
        reject("a Hamiltonian cycle exists, but output claims none")

    tokens = out_text.split()
    if len(tokens) != n + 1:
        reject(f"expected {n + 1} numbers, got {len(tokens)}")
    try:
        tour = [int(t) for t in tokens]
    except ValueError:
        reject("non-integer token in tour")

    if tour[0] != 1 or tour[-1] != 1:
        reject("tour must start and end at 1")
    if sorted(tour[:-1]) != list(range(1, n + 1)):
        reject("tour must visit every man exactly once")
    for a, b in zip(tour, tour[1:]):
        if b not in friends[a]:
            reject(f"{a} and {b} are not friends")

    sys.exit(0)


if __name__ == "__main__":
    main()
