"""Checker for p427: usage checker.py <in> <out> <ans>; exit 0 = accept.

The Yes/No verdict must match the reference. A "Yes" must come with a
Hamiltonian cycle certificate: n (the number of distinct vertices), then
the vertices along the cycle, each being an input vertex (matched after
rounding coordinates to 1e-9), all distinct, consecutive ones (and the
wrap-around pair) joined by a polyhedron edge. Any valid cycle is
accepted; the reference's own cycle is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def key3(xs, ys, zs):
    return (
        round(float(xs) * 1e9),
        round(float(ys) * 1e9),
        round(float(zs) * 1e9),
    )


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    f = int(in_tokens[pos])
    pos += 1
    vertex_id = {}
    edges = set()
    for _ in range(f):
        k = int(in_tokens[pos])
        pos += 1
        ids = []
        for _ in range(k):
            key = key3(*in_tokens[pos:pos + 3])
            pos += 3
            if key not in vertex_id:
                vertex_id[key] = len(vertex_id)
            ids.append(vertex_id[key])
        for i in range(k):
            a, b = ids[i], ids[(i + 1) % k]
            edges.add((min(a, b), max(a, b)))
    n_total = len(vertex_id)

    expected = ans_tokens[0].lower()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be Yes/No, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "no":
        sys.exit(0)

    if len(out_tokens) < 2:
        reject("missing vertex count")
    try:
        n = int(out_tokens[1])
    except ValueError:
        reject(f"vertex count is not an integer: '{out_tokens[1]}'")
    if n != n_total:
        reject(f"polyhedron has {n_total} vertices, output claims {n}")
    coords = out_tokens[2:]
    if len(coords) < 3 * n:
        reject("truncated cycle: fewer than n coordinate triples")

    cycle = []
    for i in range(n):
        try:
            key = key3(*coords[3 * i:3 * i + 3])
        except ValueError:
            reject(f"vertex {i + 1}: non-numeric coordinates")
        if key not in vertex_id:
            reject(f"vertex {i + 1} is not a vertex of the polyhedron")
        cycle.append(vertex_id[key])

    if len(set(cycle)) != n:
        reject("cycle visits some vertex more than once")
    for i in range(n):
        a, b = cycle[i], cycle[(i + 1) % n]
        if (min(a, b), max(a, b)) not in edges:
            reject(f"consecutive cycle vertices {i + 1} and {(i + 1) % n + 1} "
                   "are not joined by a polyhedron edge")

    sys.exit(0)


if __name__ == "__main__":
    main()
