"""Checker for p371: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic, so "No solution" must match the reference.
Otherwise the output is a certificate validated against the input only:
K circles of 3..10 distinct stations whose intersection graph is a path
with single-station links, radial lines as fresh terminal + a connector
lying on exactly one circle, every station on at most two lines, all N
stations used and exactly M segments. The reference's plan itself is
never compared against.
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
    expected_unsolvable = ans_tokens[0].lower() == "no"

    if not out_tokens:
        reject("empty output")
    if [t.lower() for t in out_tokens] == ["no", "solution"]:
        if expected_unsolvable:
            sys.exit(0)
        reject("a valid plan exists, got 'No solution'")
    if expected_unsolvable:
        reject("expected 'No solution'")

    pos = 0

    def next_int(what):
        nonlocal pos
        if pos >= len(out_tokens):
            reject(f"unexpected end of output while reading {what}")
        try:
            value = int(out_tokens[pos])
        except ValueError:
            reject(f"non-integer token '{out_tokens[pos]}' for {what}")
        pos += 1
        return value

    k = next_int("number of circle lines")
    if k < 1:
        reject(f"need at least one circle line, got K={k}")

    # circles_of[v] lists the circle indices containing station v;
    # in_radial[v] marks stations already used by some radial line.
    circles_of = [[] for _ in range(n + 1)]
    circles = []
    total_circle_stations = 0
    for ci in range(k):
        size = next_int(f"size of circle {ci + 1}")
        if not (3 <= size <= 10):
            reject(f"circle {ci + 1} has {size} stations, allowed 3..10")
        stations = [next_int(f"station of circle {ci + 1}") for _ in range(size)]
        if any(not (1 <= v <= n) for v in stations):
            reject(f"circle {ci + 1} uses a station outside 1..{n}")
        if len(set(stations)) != size:
            reject(f"circle {ci + 1} repeats a station")
        for v in stations:
            circles_of[v].append(ci)
            if len(circles_of[v]) > 2:
                reject(f"station {v} lies on more than two circle lines")
        circles.append(stations)
        total_circle_stations += size

    r = next_int("number of radial lines")
    if r < 0:
        reject(f"negative radial count {r}")
    in_radial = [False] * (n + 1)
    for ri in range(r):
        x = next_int(f"radial {ri + 1} station")
        y = next_int(f"radial {ri + 1} station")
        if not (1 <= x <= n and 1 <= y <= n) or x == y:
            reject(f"radial {ri + 1} stations ({x}, {y}) invalid")
        if in_radial[x] or in_radial[y]:
            reject(f"radial {ri + 1} shares a station with another radial")
        on_circle = [v for v in (x, y) if circles_of[v]]
        if len(on_circle) != 1:
            reject(
                f"radial {ri + 1}: exactly one of ({x}, {y}) must lie on a circle"
            )
        if len(circles_of[on_circle[0]]) != 1:
            reject(
                f"station {on_circle[0]} would connect more than two lines"
            )
        in_radial[x] = in_radial[y] = True

    if pos != len(out_tokens):
        reject("trailing output after the last radial line")

    if total_circle_stations + r != m:
        reject(
            f"plan has {total_circle_stations + r} segments, required {m}"
        )
    for v in range(1, n + 1):
        if not circles_of[v] and not in_radial[v]:
            reject(f"station {v} is not used by any line")

    # The circle intersection graph must be a simple path: every shared
    # station yields one adjacency edge, no pair may share two stations.
    adj_edges = set()
    degree = [0] * k
    parent = list(range(k))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    for v in range(1, n + 1):
        if len(circles_of[v]) == 2:
            a, b = circles_of[v]
            if (a, b) in adj_edges:
                reject(
                    f"circles {a + 1} and {b + 1} intersect at two stations"
                )
            adj_edges.add((a, b))
            degree[a] += 1
            degree[b] += 1
            parent[find(a)] = find(b)

    if len(adj_edges) != k - 1:
        reject(f"circle chain needs {k - 1} links, found {len(adj_edges)}")
    if any(d > 2 for d in degree):
        reject("a circle intersects more than two other circles")
    if k > 1 and len({find(i) for i in range(k)}) != 1:
        reject("circle lines do not form a single connected chain")

    sys.exit(0)


if __name__ == "__main__":
    main()
