"""Tests for p503 (Running City): shortest S->T time with route penalties.

Directed weighted graph, outdegree of every vertex <= 10. Special routes
are simple paths (edge-id sequences whose consecutive edges share a
vertex and which repeat no vertex); the sum of their lengths is <= 2m and
each edge appears in <= 10 routes. Crossing a route as a contiguous
subpath of the walk costs its time again.

Seed 1 is the minimal graph (2 nodes, 1 edge, no routes). Seeds 2 and 3
are maximal: n = m_per_out * n vertices with outdegree pinned at 10 and
the route budget filled to ~2m. Seed 2 is a dense random layered graph;
seed 3 chains routes heavily over long simple paths so the automaton and
penalty bookkeeping are stressed. Later seeds randomise size, weights,
reachability (sometimes T is unreachable) and route density.
"""

import random
import sys

N_MAX = 700
M_MAX = 5000
C_MAX = 1000
OUTDEG = 10

seed = int(sys.argv[1])
rng = random.Random(seed)


def build(n, target_m, c_max, route_budget, route_edge_cap, long_routes):
    # adjacency with outdegree cap; edges stored as (a, b, c)
    edges = []
    outdeg = [0] * (n + 1)
    edge_id = {}  # (a,b) -> id to avoid duplicate parallel for simplicity

    # First lay a Hamiltonian-ish chain 1->2->...->n so the graph is connected
    for v in range(1, n):
        if outdeg[v] < OUTDEG:
            c = rng.randint(1, c_max)
            edge_id[(v, v + 1)] = len(edges)
            edges.append((v, v + 1, c))
            outdeg[v] += 1

    # add random forward/backward edges up to target_m, respecting outdegree
    attempts = 0
    while len(edges) < target_m and attempts < target_m * 40 + 1000:
        attempts += 1
        a = rng.randint(1, n)
        if outdeg[a] >= OUTDEG:
            continue
        b = rng.randint(1, n)
        if b == a:
            continue
        if (a, b) in edge_id:
            continue
        c = rng.randint(1, c_max)
        edge_id[(a, b)] = len(edges)
        edges.append((a, b, c))
        outdeg[a] += 1

    m = len(edges)

    # adjacency for route extraction: out_edges[v] = list of (edge_id, b)
    out_edges = [[] for _ in range(n + 1)]
    for i, (a, b, c) in enumerate(edges):
        out_edges[a].append((i, b))

    # build special routes: random simple-path walks over existing edges
    routes = []
    edge_use = [0] * m
    used_len = 0
    tries = 0
    while used_len < route_budget and tries < route_budget * 8 + 200:
        tries += 1
        start = rng.randint(1, n)
        seq = []
        visited = {start}
        cur = start
        maxlen = rng.randint(2, 8) if long_routes else rng.randint(1, 3)
        while len(seq) < maxlen:
            choices = [(eid, b) for (eid, b) in out_edges[cur]
                       if b not in visited and edge_use[eid] < route_edge_cap]
            if not choices:
                break
            eid, b = rng.choice(choices)
            seq.append(eid)
            visited.add(b)
            cur = b
        if not seq:
            continue
        if used_len + len(seq) > route_budget:
            break
        for eid in seq:
            edge_use[eid] += 1
        routes.append(seq)
        used_len += len(seq)

    return n, edges, routes


def emit(n, edges, routes, S, T):
    out = [f"{n} {len(edges)} {len(routes)} {S} {T}"]
    for a, b, c in edges:
        out.append(f"{a} {b} {c}")
    for seq in routes:
        out.append(str(len(seq)) + " " + " ".join(str(e + 1) for e in seq))
    sys.stdout.write("\n".join(out))
    sys.stdout.write("\n")


if seed == 1:
    sys.stdout.write("2 1 0 1 2\n1 2 5\n")
    sys.exit()

if seed == 2:
    n = N_MAX
    target_m = M_MAX
    n, edges, routes = build(n, target_m, C_MAX, 2 * target_m, 10, long_routes=False)
    emit(n, edges, routes, 1, n)
    sys.exit()

if seed == 3:
    n = N_MAX
    target_m = M_MAX
    n, edges, routes = build(n, target_m, C_MAX, 2 * target_m, 10, long_routes=True)
    emit(n, edges, routes, 1, n)
    sys.exit()

if seed == 4:
    # T deliberately unreachable: all edges stay within nodes 1..n-1, but we
    # set the finish to the isolated node n (which has no incoming edge)
    n = rng.randint(3, 200)
    target_m = rng.randint(n, min(OUTDEG * (n - 1), 800))
    _, edges, routes = build(n - 1, target_m, C_MAX, 2 * target_m, 10, long_routes=False)
    emit(n, edges, routes, 1, n)
    sys.exit()

# randomized mid-size
n = rng.randint(2, 300)
target_m = rng.randint(n - 1, min(OUTDEG * n, 1500))
c_max = rng.choice([1, 10, 1000, C_MAX])
budget = rng.randint(0, 2 * target_m)
n, edges, routes = build(n, target_m, c_max, budget, 10, long_routes=(seed % 2 == 0))
S = 1
# sometimes pick an unreachable / far T
T = rng.randint(2, n)
emit(n, edges, routes, S, T)
