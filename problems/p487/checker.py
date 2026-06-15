"""Checker for p487: usage checker.py <in> <out> <ans>; exit 0 = accept.

The Yes/No part is deterministic, so it must match the reference. A "Yes"
must be accompanied by a route: a cyclic ordering of all 4N gates such
that, for every gate, one of its two route-neighbours is its road partner
and the other lies in the same town. That makes the route alternate road
and internal-town moves, visiting each gate exactly once and returning to
the start. Any such route is accepted; the reference's own route is never
compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    if not ans_tokens:
        reject("empty reference answer")
    expected = ans_tokens[0].lower()

    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be Yes/No, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")

    if got == "no":
        if len(out_tokens) > 1:
            reject("trailing output after 'No'")
        sys.exit(0)

    n = int(in_tokens[0])
    g = 4 * n

    road = [0] * (g + 1)
    idx = 1
    for _ in range(2 * n):
        a = int(in_tokens[idx])
        b = int(in_tokens[idx + 1])
        idx += 2
        if not (1 <= a <= g) or not (1 <= b <= g):
            reject("road gate out of range in input")
        road[a] = b
        road[b] = a

    route = out_tokens[1:]
    if len(route) != g:
        reject(f"expected {g} gates in the route, got {len(route)}")

    try:
        seq = [int(x) for x in route]
    except ValueError:
        reject("route contains a non-integer token")

    if any(not (1 <= x <= g) for x in seq):
        reject("route gate out of range")
    if len(set(seq)) != g:
        reject("route does not visit every gate exactly once")

    # The courier starts in the capital (town 1, gates 1..4).
    if not (1 <= seq[0] <= 4):
        reject(f"route must start at a capital gate (1..4), got {seq[0]}")

    def town(x):
        return (x - 1) // 4

    for i in range(g):
        cur = seq[i]
        prev = seq[(i - 1) % g]
        nxt = seq[(i + 1) % g]
        neighbours = [prev, nxt]

        road_hits = [h for h in neighbours if h == road[cur]]
        if len(road_hits) != 1:
            reject(
                f"gate {cur} must use its road partner {road[cur]} as exactly "
                f"one neighbour; neighbours are {prev} and {nxt}"
            )

        other = nxt if prev == road[cur] else prev
        if town(other) != town(cur):
            reject(
                f"gate {cur} internal move to {other} is not within the same "
                f"town"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
