"""Tests for p336 (Elections).

N <= 1e5 parties, M <= 2e5 'a knows about b' facts (self-facts and
duplicates allowed), Q <= 2e5 queries. Type-2 queries must reference two
distinct LIVE ids (never an id already merged away), so a live-id pool
is maintained with swap-remove; merged blocks get ids N+1, N+2, ...

Seed 2 is maximal with uniform random facts and a 50/50 query mix; seed
3 is maximal with hub-centred facts and merge-heavy queries collapsing
everything into one giant block (worst case for small-to-large).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
    print(1, 1)
    print(1)
    print(1, 1, 1)
    sys.exit()

if seed == 2:
    n, m, q = 100000, 200000, 200000
    merge_prob = 0.5
    hubs = None
elif seed == 3:
    n, m, q = 100000, 200000, 200000
    merge_prob = 0.75
    hubs = rng.sample(range(1, n + 1), 20)
else:
    n = rng.randint(1, 2000)
    m = rng.randint(1, 8000)
    q = rng.randint(1, 8000)
    merge_prob = rng.uniform(0.1, 0.9)
    hubs = rng.sample(range(1, n + 1), min(n, 5)) if seed % 2 else None

out = [f"{n} {m}"]
for _ in range(m):
    if hubs and rng.random() < 0.7:
        a = rng.choice(hubs)
        b = rng.randint(1, n)
        if rng.random() < 0.5:
            a, b = b, a
    else:
        a = rng.randint(1, n)
        b = rng.randint(1, n)
    out.append(f"{a} {b}")

live = list(range(1, n + 1))
next_id = n + 1
queries = []
for _ in range(q):
    if len(live) >= 2 and rng.random() < merge_prob:
        i = rng.randrange(len(live))
        a = live[i]
        live[i] = live[-1]
        live.pop()
        j = rng.randrange(len(live))
        b = live[j]
        live[j] = next_id
        queries.append(f"2 {a} {b}")
        next_id += 1
    else:
        a = rng.choice(live)
        b = rng.choice(live)
        queries.append(f"1 {a} {b}")

out.append(str(len(queries)))
out.extend(queries)
print("\n".join(out))
