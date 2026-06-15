"""Checker for p185: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility is deterministic: if the reference printed "No solution" the
output must say the same. Otherwise the output must be two routes from
junction 1 to junction N, each of total length equal to the shortest
distance (recomputed here by Dijkstra), using only existing roads and
never using one road twice across both routes. Any such pair is
accepted; the reference's own routes are never compared against.
"""

import heapq
import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    weight = {}
    adj = [[] for _ in range(n + 1)]
    for i in range(m):
        x = int(in_tokens[2 + 3 * i])
        y = int(in_tokens[3 + 3 * i])
        l = int(in_tokens[4 + 3 * i])
        weight[(x, y)] = l
        weight[(y, x)] = l
        adj[x].append((y, l))
        adj[y].append((x, l))

    no_solution = [t.lower() for t in ans_tokens] == ["no", "solution"]
    out_tokens_lower = [t.lower() for t in out_text.split()]

    if no_solution:
        if out_tokens_lower != ["no", "solution"]:
            reject("expected 'No solution'")
        sys.exit(0)
    if out_tokens_lower[:2] == ["no", "solution"]:
        reject("claimed 'No solution' but two routes exist")

    # shortest distance from 1 to n
    INF = float("inf")
    dist = [INF] * (n + 1)
    dist[1] = 0
    pq = [(0, 1)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v, l in adj[u]:
            if dist[v] > d + l:
                dist[v] = d + l
                heapq.heappush(pq, (dist[v], v))
    shortest = dist[n]

    lines = [ln for ln in out_text.splitlines() if ln.split()]
    if len(lines) != 2:
        reject(f"expected exactly 2 route lines, got {len(lines)}")

    used = set()
    for which, line in enumerate(lines, 1):
        try:
            path = [int(t) for t in line.split()]
        except ValueError:
            reject(f"route {which}: non-integer junction")
        if len(path) < 2:
            reject(f"route {which}: too short")
        if path[0] != 1 or path[-1] != n:
            reject(f"route {which}: must go from 1 to {n}")
        if any(not (1 <= v <= n) for v in path):
            reject(f"route {which}: junction out of range")
        total = 0
        for a, b in zip(path, path[1:]):
            if (a, b) not in weight:
                reject(f"route {which}: no road between {a} and {b}")
            key = (min(a, b), max(a, b))
            if key in used:
                reject(f"road {key} used twice")
            used.add(key)
            total += weight[(a, b)]
        if total != shortest:
            reject(f"route {which}: length {total} != shortest {shortest}")

    sys.exit(0)


if __name__ == "__main__":
    main()
