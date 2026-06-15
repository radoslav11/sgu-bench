"""Tests for p172 (eXam): is the subject graph bipartite, and one side.

N <= 200 subjects, M <= 30000 pupil pairs. Even seeds build a random
bipartite graph (answer "yes"), odd seeds plant an odd cycle so the
answer is "no". Seed 1 is the minimal case, seed 2 the maximal one.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2, 1)
    print(1, 2)  # minimal valid case -> "yes", day one = either subject
    sys.exit()

n = 200 if seed == 2 else rng.randint(2, 200)
m_target = 30000 if seed == 2 else rng.randint(1, 2000)

side = [rng.randint(0, 1) for _ in range(n + 1)]
left = [v for v in range(1, n + 1) if side[v] == 0]
right = [v for v in range(1, n + 1) if side[v] == 1]
if not left or not right:
    left, right = [1], list(range(2, n + 1)) or [1]

edges = [(rng.choice(left), rng.choice(right)) for _ in range(m_target)]
if seed % 2 == 1 and n >= 3:
    a, b, c = rng.sample(range(1, n + 1), 3)
    edges += [(a, b), (b, c), (c, a)]

print(n, len(edges))
for u, v in edges:
    print(u, v)
