"""Checker for p138: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any schedule is accepted as long as it is internally valid: G equals
half the total game count, every game is between two distinct friends,
each game after the first involves the previous game's winner, and each
friend appears in exactly as many games as the input demands.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n = int(in_tokens[0])
    counts = [int(x) for x in in_tokens[1 : 1 + n]]
    total_games = sum(counts) // 2

    if not out_tokens:
        reject("empty output")
    try:
        values = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    g = values[0]
    if g != total_games:
        reject(f"G must be {total_games}, got {g}")
    if len(values) != 1 + 2 * g:
        reject(f"expected {2 * g} integers after G, got {len(values) - 1}")

    played = [0] * (n + 1)
    prev_winner = None
    for i in range(g):
        w, l = values[1 + 2 * i], values[2 + 2 * i]
        if not (1 <= w <= n and 1 <= l <= n):
            reject(f"game {i + 1}: player out of range ({w}, {l})")
        if w == l:
            reject(f"game {i + 1}: a friend cannot play himself")
        if prev_winner is not None and prev_winner not in (w, l):
            reject(f"game {i + 1}: previous winner {prev_winner} not playing")
        played[w] += 1
        played[l] += 1
        prev_winner = w

    for friend in range(1, n + 1):
        if played[friend] != counts[friend - 1]:
            reject(
                f"friend {friend} played {played[friend]} games, "
                f"expected {counts[friend - 1]}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
