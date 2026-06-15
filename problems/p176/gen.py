"""Tests for p176 (Flow construction): minimal flow with lower bounds.

N <= 100 nodes, pipes are directed, at most one pipe per unordered pair
(no antiparallel pipes, no self loops, no pipe between 1 and N),
capacities 1..10^5, C = 1 pipes must run exactly full.

Feasible tests are built backwards: orient edges along a random
topological order with node 1 first and node N last, push random 1->N
paths to accumulate a witness flow, then set cap = flow on a fraction
of used edges and mark those C = 1. Seed 2 is the maximal complete DAG
(M = 4949) built this way; seed 3 is the same size but with tiny random
capacities and random mandatory pipes (usually infeasible). Other seeds
cover the two samples, a mandatory cycle that needs zero production
speed, a guaranteed-impossible dead end, and random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

CAP = 10**5


def emit(n, pipe_list):
    print(n, len(pipe_list))
    for u, v, z, c in pipe_list:
        print(u, v, z, c)


def feasible_case(rng, n, want_edges, n_paths, saturate_prob):
    # positions: order[0] = node 1, order[-1] = node N (1-indexed ids)
    mid = list(range(2, n))
    rng.shuffle(mid)
    order = [1] + mid + [n]
    pairs = [
        (i, j)
        for i in range(n)
        for j in range(i + 1, n)
        if not (i == 0 and j == n - 1)
    ]
    rng.shuffle(pairs)
    chosen = set(pairs[:want_edges])
    # make sure a 1 -> N backbone exists for the paths
    flow = {}
    for _ in range(n_paths):
        pos = 0
        path = []
        while pos != n - 1:
            nxt = rng.randint(pos + 1, n - 1)
            if pos == 0 and nxt == n - 1:
                continue
            path.append((pos, nxt))
            pos = nxt
        chosen.update(path)
        amount = rng.randint(1, 1000)
        amount = min([amount] + [CAP - flow.get(e, 0) for e in path])
        if amount <= 0:
            continue
        for e in path:
            flow[e] = flow.get(e, 0) + amount
    pipe_list = []
    for i, j in chosen:
        f = flow.get((i, j), 0)
        if f > 0 and rng.random() < saturate_prob:
            z, c = f, 1
        elif f > 0:
            z, c = min(CAP, f + rng.randint(1, CAP)), 0
        else:
            z, c = rng.randint(1, CAP), 0
        pipe_list.append((order[i], order[j], z, c))
    rng.shuffle(pipe_list)
    return pipe_list


if seed == 1:
    emit(3, [(1, 2, 1, 1), (2, 3, 1, 0)])
elif seed == 2:
    # maximal: complete DAG on 100 nodes, M = 4949, heavy lower bounds
    emit(100, feasible_case(rng, 100, 4949, 600, 0.6))
elif seed == 3:
    # maximal size, tiny random capacities, random mandatory pipes
    n = 100
    pairs = [
        (i + 1, j + 1)
        for i in range(n)
        for j in range(i + 1, n)
        if not (i == 0 and j == n - 1)
    ]
    pipe_list = []
    for u, v in pairs:
        if rng.random() < 0.5:
            u, v = v, u
        pipe_list.append((u, v, rng.randint(1, 3), int(rng.random() < 0.05)))
    rng.shuffle(pipe_list)
    emit(n, pipe_list)
elif seed == 4:
    emit(4, [(1, 2, 2, 0), (2, 4, 1, 1), (1, 3, 2, 1), (3, 4, 3, 0)])
elif seed == 5:
    emit(4, [(1, 2, 1, 0), (2, 4, 2, 1), (1, 3, 3, 1), (3, 4, 2, 0)])
elif seed == 6:
    # mandatory cycle among middle nodes: minimal speed is 0
    n = 50
    cyc = list(range(2, n))
    rng.shuffle(cyc)
    cyc = cyc[:20]
    z = rng.randint(1, CAP)
    pipe_list = [
        (cyc[i], cyc[(i + 1) % len(cyc)], z, 1) for i in range(len(cyc))
    ]
    pipe_list += [(1, cyc[0], rng.randint(1, CAP), 0),
                  (cyc[1], n, rng.randint(1, CAP), 0)]
    rng.shuffle(pipe_list)
    emit(n, pipe_list)
elif seed == 7:
    # impossible: mandatory pipe into a node with no way out
    n = 30
    dead = 2
    pipe_list = [(1, dead, rng.randint(1, CAP), 1)]
    for i in range(3, n):
        pipe_list.append((1, i, rng.randint(1, CAP), 0))
        pipe_list.append((i, n, rng.randint(1, CAP), 0))
    rng.shuffle(pipe_list)
    emit(n, pipe_list)
elif seed in (8, 9):
    n = rng.randint(10, 100)
    max_m = n * (n - 1) // 2 - 1
    emit(n, feasible_case(rng, n, rng.randint(n, max_m),
                          rng.randint(10, 200), rng.uniform(0.2, 0.8)))
else:
    # fully random orientation and flags; may be feasible or not
    n = rng.randint(5, 100)
    pairs = [
        (i + 1, j + 1)
        for i in range(n)
        for j in range(i + 1, n)
        if not (i == 0 and j == n - 1)
    ]
    rng.shuffle(pairs)
    m = rng.randint(min(4, len(pairs)), len(pairs))
    pipe_list = []
    for u, v in pairs[:m]:
        if rng.random() < 0.5:
            u, v = v, u
        pipe_list.append(
            (u, v, rng.randint(1, CAP), int(rng.random() < 0.12))
        )
    emit(n, pipe_list)
