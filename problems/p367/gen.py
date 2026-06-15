"""Tests for p367 (Contest): scheduling with prerequisites t[a] <= t[b].

N <= 1000 problems, t_i <= 3500, M <= 10000 constraints, T <= 1e9.
Edges always go from a cheaper (or equal) problem to a costlier one;
cycles are planted only among equal-time problems so the t[a] <= t[b]
guarantee holds around the cycle. Seed 1 is minimal, seeds 2-3 maximal.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
    print(1)
    print(0)
    sys.exit()

if seed == 2:
    # Maximal: N=1000, M=10000, distinct-ish times, T lets ~everything fit.
    n, m, big_t = 1000, 10000, 10**9
    t = [rng.randint(1, 3500) for _ in range(n)]
elif seed == 3:
    # Maximal with heavy ties: few distinct time values, plus cycles among
    # equal-time problems, tight T cutting the schedule in the middle.
    n, m, big_t = 1000, 10000, 350 * 1000 // 2
    t = [rng.choice([7, 7, 7, 100, 100, 3500]) for _ in range(n)]
elif seed == 4:
    # All times equal: order is dictated purely by the constraint DAG.
    n, m, big_t = 800, 9000, rng.randint(1, 800 * 50)
    t = [50] * n
else:
    n = rng.randint(2, 1000)
    m = rng.randint(0, 10000)
    big_t = rng.choice([1, rng.randint(1, 3500 * n), 10**9])
    t = [rng.randint(1, 3500) for _ in range(n)]

# Order problems by time; an edge (a -> b) is only drawn from an earlier
# position to a later one in this order, guaranteeing t[a] <= t[b].
idx = sorted(range(n), key=lambda i: t[i])
edges = []
while len(edges) < m:
    i, j = rng.randrange(n), rng.randrange(n)
    if i == j:
        continue
    if i > j:
        i, j = j, i
    a, b = idx[i], idx[j]
    if seed % 2 == 1 and t[a] == t[b] and rng.random() < 0.05:
        a, b = b, a  # back edge among equal times: may create cycles
    edges.append((a + 1, b + 1))

print(n, big_t)
print(" ".join(map(str, t)))
print(len(edges))
for a, b in edges:
    print(a, b)
