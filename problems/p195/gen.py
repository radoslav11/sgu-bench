"""Tests for p195 (New Year Bonus Grant): rooted tree, 2 <= N <= 500000.

Input: N, then N-1 parents (parent of node i+1, always < i+1).
Seed 1 is minimal, seeds 2-4 are maximal trees with different shapes
(uniform random, chain, star), the rest are mid-size adversarial shapes
(caterpillar, binary tree, random).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 500000

if seed == 1:
    n = 2
    parents = [1]
elif seed == 2:
    # maximal, uniform random attachment (shallow, bushy)
    n = MAXN
    parents = [rng.randint(1, i) for i in range(1, n)]
elif seed == 3:
    # maximal chain: depth N-1, kills naive recursion
    n = MAXN
    parents = [i for i in range(1, n)]
elif seed == 4:
    # maximal star: node 1 has N-1 children
    n = MAXN
    parents = [1] * (n - 1)
elif seed == 5:
    # caterpillar: long spine, each spine node has a few leaves
    n = 300000
    parents = []
    spine = [1]
    while len(parents) < n - 1:
        nxt = len(parents) + 2
        parents.append(spine[-1])
        spine.append(nxt)
        for _ in range(rng.randint(0, 3)):
            if len(parents) >= n - 1:
                break
            parents.append(spine[-2])
elif seed == 6:
    # complete binary tree
    n = 262143
    parents = [i // 2 for i in range(2, n + 1)]
elif seed == 7:
    # deep-ish random: parent close to the node
    n = 200000
    parents = [max(1, i - rng.randint(1, 3)) for i in range(1, n)]
else:
    n = rng.randint(2, 5000)
    parents = [rng.randint(1, i) for i in range(1, n)]

print(n)
print(" ".join(map(str, parents)))
