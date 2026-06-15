"""Tests for p322 (The Great Union): two labeled trees on N <= 2000
vertices, N-1 edges each, given one edge per line.

Seed 1 is N=1 (no edges, zero months), seed 2 two unrelated maximal
random trees, seed 3 two maximal paths with different vertex orders
(long BFS cycles), later seeds share most edges (few months needed),
use stars vs paths, or identical trees.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def random_tree(n, shape="random"):
    edges = []
    if shape == "path":
        perm = list(range(1, n + 1))
        rng.shuffle(perm)
        edges = [(perm[i], perm[i + 1]) for i in range(n - 1)]
    elif shape == "star":
        c = rng.randint(1, n)
        edges = [(c, v) for v in range(1, n + 1) if v != c]
    else:
        for i in range(2, n + 1):
            edges.append((rng.randint(1, i - 1), i))
    rng.shuffle(edges)
    return edges


def perturb(edges, n, k):
    # k times: drop a random edge and reconnect the two components
    es = [tuple(e) for e in edges]
    for _ in range(k):
        i = rng.randrange(len(es))
        u0, v0 = es.pop(i)
        adj = [[] for _ in range(n + 1)]
        for u, v in es:
            adj[u].append(v)
            adj[v].append(u)
        comp = [0] * (n + 1)
        stack = [u0]
        comp[u0] = 1
        while stack:
            x = stack.pop()
            for y in adj[x]:
                if not comp[y]:
                    comp[y] = 1
                    stack.append(y)
        side_a = [v for v in range(1, n + 1) if comp[v]]
        side_b = [v for v in range(1, n + 1) if not comp[v]]
        es.append((rng.choice(side_a), rng.choice(side_b)))
    rng.shuffle(es)
    return es


def emit(n, e1, e2):
    lines = [str(n)]
    for u, v in e1:
        lines.append(f"{u} {v}")
    for u, v in e2:
        lines.append(f"{u} {v}")
    sys.stdout.write("\n".join(lines) + "\n")


if seed == 1:
    print(1)
    sys.exit()

if seed == 2:
    n = 2000
    emit(n, random_tree(n), random_tree(n))
    sys.exit()

if seed == 3:
    n = 2000
    emit(n, random_tree(n, "path"), random_tree(n, "path"))
    sys.exit()

if seed == 4:
    n = 2000
    emit(n, random_tree(n, "star"), random_tree(n, "path"))
    sys.exit()

n = rng.randint(2, 2000)
mode = seed % 3
if mode == 0:
    base = random_tree(n)
    emit(n, base, perturb(base, n, rng.randint(0, 10)))
elif mode == 1:
    t = random_tree(n)
    t2 = [tuple(e) for e in t]
    rng.shuffle(t2)
    emit(n, t, t2)
else:
    shapes = ["random", "path", "star"]
    emit(n, random_tree(n, rng.choice(shapes)), random_tree(n, rng.choice(shapes)))
