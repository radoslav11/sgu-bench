"""Tests for p398 (Friends of Friends): N <= 50 users, symmetric
friendship lists, find friends-of-friends of user x.

Seed 1 is a single lonely user, seed 2 the complete graph on 50 users,
seed 3 a dense random graph on 50 users. Later seeds cover stars, paths
(x in the middle), users with zero friends, and random densities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, x, edges):
    adj = [set() for _ in range(n + 1)]
    for a, b in edges:
        adj[a].add(b)
        adj[b].add(a)
    print(n, x)
    for i in range(1, n + 1):
        friends = sorted(adj[i])
        rng.shuffle(friends)
        print(len(friends), *friends)


if seed == 1:
    emit(1, 1, [])
elif seed == 2:
    n = 50
    emit(n, rng.randint(1, n),
         [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)])
elif seed == 3:
    n = 50
    edges = [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)
             if rng.random() < 0.5]
    emit(n, rng.randint(1, n), edges)
elif seed == 4:
    # star: x is the center, every friend-of-friend question degenerates
    n = 50
    emit(n, 1, [(1, i) for i in range(2, n + 1)])
elif seed == 5:
    # path with x in the middle: exactly two friends of friends
    n = 50
    emit(n, 25, [(i, i + 1) for i in range(1, n)])
elif seed == 6:
    # x has no friends at all
    n = 50
    edges = [(a, b) for a in range(2, n + 1) for b in range(a + 1, n + 1)
             if rng.random() < 0.3]
    emit(n, 1, edges)
else:
    n = rng.randint(2, 50)
    p = rng.uniform(0.05, 0.8)
    edges = [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)
             if rng.random() < p]
    emit(n, rng.randint(1, n), edges)
