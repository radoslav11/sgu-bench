"""Tests for p384 (Country): the reformed graph is a friendship / windmill
graph -- a center vertex c with k pairwise-disjoint triangles all sharing
c. So n = 2k + 1 vertices and m = 3k edges. Operations run until EOF:
"DELETE x" removes road x (1-based edge index, each edge deleted at most
once), "LENGTH x y" asks the current shortest path length.

Seed 1 is the minimal single triangle (n=3). Seeds 2 and 3 are maximal:
seed 2 has k=50000 triangles (n=100001, m=150000) with 100000 queries
heavy on LENGTH after interleaved DELETEs; seed 3 is the same size but
deletes every edge while querying, exercising the -1 / disconnection paths
across the whole structure. The rest are randomized small/medium windmills
with shuffled labels (center is not vertex 1), mixed DELETE/LENGTH streams,
same-triangle and cross-triangle and center-involving queries.

Vertex labels and edge order are permuted so nothing leaks the windmill
structure through the test layout.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def build_windmill(k):
    """Return (n, perm_label, center_label, edges, triangles).
    edges is a shuffled list of (u, v) with permuted labels. triangles is a
    list of (a, b) partner pairs in permuted labels. center_label is the
    permuted center label."""
    n = 2 * k + 1
    # internal labels: 0 = center, triangles use (2i+1, 2i+2)
    labels = list(range(1, n + 1))
    rng.shuffle(labels)
    # map internal id -> output label
    out_label = {}
    out_label[0] = labels[0]
    pairs = []
    for i in range(k):
        a = labels[1 + 2 * i]
        b = labels[2 + 2 * i]
        out_label[2 * i + 1] = a
        out_label[2 * i + 2] = b
        pairs.append((a, b))
    center = out_label[0]
    edges = []
    for (a, b) in pairs:
        edges.append((center, a))
        edges.append((center, b))
        edges.append((a, b))
    rng.shuffle(edges)
    # randomly flip endpoint order
    edges = [(u, v) if rng.random() < 0.5 else (v, u) for (u, v) in edges]
    return n, center, edges, pairs


def emit(n, edges, ops):
    out = [f"{n} {len(edges)}"]
    out.extend(f"{u} {v}" for u, v in edges)
    out.extend(ops)
    sys.stdout.write("\n".join(out) + "\n")


def random_ops(n, edges, center, pairs, num_ops, delete_prob):
    """Generate an op stream; each edge deleted at most once."""
    m = len(edges)
    deleted = [False] * m
    not_deleted = list(range(m))
    ops = []
    # vertices for LENGTH: gather useful vertices
    all_v = list(range(1, n + 1))
    for _ in range(num_ops):
        if not_deleted and rng.random() < delete_prob:
            # pick an undeleted edge index uniformly
            idx_pos = rng.randrange(len(not_deleted))
            e = not_deleted[idx_pos]
            not_deleted[idx_pos] = not_deleted[-1]
            not_deleted.pop()
            deleted[e] = True
            ops.append(f"DELETE {e + 1}")
        else:
            x = rng.choice(all_v)
            y = rng.choice(all_v)
            ops.append(f"LENGTH {x} {y}")
    return ops


if seed == 1:
    # single triangle
    n, center, edges, pairs = build_windmill(1)
    a, b = pairs[0]
    ops = [
        f"LENGTH {a} {b}",
        "DELETE 1",
        f"LENGTH {a} {b}",
        f"LENGTH {a} {center}",
        f"LENGTH {center} {b}",
    ]
    emit(n, edges, ops)
elif seed == 2:
    k = 50000
    n, center, edges, pairs = build_windmill(k)
    ops = random_ops(n, edges, center, pairs, 100000, delete_prob=0.3)
    emit(n, edges, ops)
elif seed == 3:
    # delete every edge, interleaving LENGTH queries -> heavy -1 paths.
    k = 50000
    n, center, edges, pairs = build_windmill(k)
    m = len(edges)
    order = list(range(m))
    rng.shuffle(order)
    all_v = list(range(1, n + 1))
    ops = []
    di = 0
    # 50000 deletes + 50000 queries interleaved = 100000 ops
    for step in range(100000):
        if di < m and (step % 2 == 0):
            ops.append(f"DELETE {order[di] + 1}")
            di += 1
        else:
            x = rng.choice(all_v)
            y = rng.choice(all_v)
            ops.append(f"LENGTH {x} {y}")
    emit(n, edges, ops)
elif seed == 4:
    k = rng.randint(1, 20)
    n, center, edges, pairs = build_windmill(k)
    ops = random_ops(n, edges, center, pairs, 60, delete_prob=0.4)
    emit(n, edges, ops)
elif seed == 5:
    # medium, query-heavy
    k = rng.randint(1000, 3000)
    n, center, edges, pairs = build_windmill(k)
    ops = random_ops(n, edges, center, pairs, 20000, delete_prob=0.1)
    emit(n, edges, ops)
elif seed == 6:
    # same-triangle queries emphasized (partner pairs and center)
    k = rng.randint(50, 500)
    n, center, edges, pairs = build_windmill(k)
    ops = []
    m = len(edges)
    not_deleted = list(range(m))
    for _ in range(2 * k + 200):
        r = rng.random()
        if r < 0.3 and not_deleted:
            pos = rng.randrange(len(not_deleted))
            e = not_deleted[pos]
            not_deleted[pos] = not_deleted[-1]
            not_deleted.pop()
            ops.append(f"DELETE {e + 1}")
        elif r < 0.65:
            a, b = rng.choice(pairs)
            ops.append(f"LENGTH {a} {b}")
        else:
            a, b = rng.choice(pairs)
            ops.append(f"LENGTH {a} {center}" if rng.random() < 0.5
                       else f"LENGTH {center} {b}")
    emit(n, edges, ops)
elif seed == 7:
    # all LENGTH, no deletions: pure shortest-path on full windmill
    k = rng.randint(500, 2000)
    n, center, edges, pairs = build_windmill(k)
    all_v = list(range(1, n + 1))
    ops = [f"LENGTH {rng.choice(all_v)} {rng.choice(all_v)}"
           for _ in range(5000)]
    emit(n, edges, ops)
elif seed == 8:
    # delete-heavy then query
    k = rng.randint(2000, 5000)
    n, center, edges, pairs = build_windmill(k)
    ops = random_ops(n, edges, center, pairs, 30000, delete_prob=0.5)
    emit(n, edges, ops)
else:
    k = rng.randint(1, 5)
    n, center, edges, pairs = build_windmill(k)
    ops = random_ops(n, edges, center, pairs, 30, delete_prob=0.35)
    emit(n, edges, ops)
