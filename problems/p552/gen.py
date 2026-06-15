"""Tests for p552 (Database optimization).

- n <= 5e4 objects, each with 1..4 properties key=value.
- m <= 1e5 queries, each with 1..4 properties.
- keys/values are 1..5 chars of digits and lowercase letters; keys within
  one object/query are distinct.

Seed plan:

- seed 1: minimal input (one object, one query).

- seed 2: maximal n and m where keys/values come from a small alphabet so
  queries hit many matching objects (large counts, dense hash buckets).

- seed 3: maximal n and m with mostly distinct random properties so most
  queries miss (zeros) and the property universe is huge.

- seeds 4+: random mid-size cases plus adversarial mixes: duplicate
  objects, queries reusing object properties, the "03" vs "3" distinction,
  all objects with exactly 4 properties (max subset blow-up).
"""

import random
import sys

ALNUM = "0123456789abcdefghijklmnopqrstuvwxyz"


def rand_token(rng, lo=1, hi=5):
    length = rng.randint(lo, hi)
    return "".join(rng.choice(ALNUM) for _ in range(length))


def make_object(rng, keys_pool, vals_pool, kmin=1, kmax=4):
    k = rng.randint(kmin, kmax)
    keys = rng.sample(keys_pool, min(k, len(keys_pool)))
    k = len(keys)
    props = []
    for key in keys:
        props.append((key, rng.choice(vals_pool)))
    return props


def emit(objects, queries):
    out = [str(len(objects))]
    for props in objects:
        toks = [str(len(props))] + [f"{k}={v}" for k, v in props]
        out.append(" ".join(toks))
    out.append(str(len(queries)))
    for props in queries:
        toks = [str(len(props))] + [f"{k}={v}" for k, v in props]
        out.append(" ".join(toks))
    sys.stdout.write("\n".join(out) + "\n")


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        emit([[("k", "v")]], [[("k", "v")]])
        return

    if seed == 2:
        # Max n, m, small alphabet -> dense matches.
        n = 50000
        m = 100000
        keys_pool = ["k1", "k2", "k3", "k4", "k5", "k6"]
        vals_pool = [str(v) for v in range(8)]
        objects = [make_object(rng, keys_pool, vals_pool, 1, 4) for _ in range(n)]
        # Queries: reuse a present object's properties or random subsets.
        queries = []
        for _ in range(m):
            if rng.random() < 0.7:
                obj = rng.choice(objects)
                l = rng.randint(1, len(obj))
                queries.append(rng.sample(obj, l))
            else:
                l = rng.randint(1, 4)
                keys = rng.sample(keys_pool, l)
                queries.append([(k, rng.choice(vals_pool)) for k in keys])
        emit(objects, queries)
        return

    if seed == 3:
        # Max n, m, mostly distinct random -> many zeros, huge universe.
        n = 50000
        m = 100000
        objects = []
        for _ in range(n):
            k = rng.randint(1, 4)
            keys = []
            while len(keys) < k:
                t = rand_token(rng)
                if t not in keys:
                    keys.append(t)
            objects.append([(key, rand_token(rng)) for key in keys])
        queries = []
        for _ in range(m):
            if rng.random() < 0.4:
                obj = rng.choice(objects)
                l = rng.randint(1, len(obj))
                queries.append(rng.sample(obj, l))
            else:
                l = rng.randint(1, 4)
                keys = []
                while len(keys) < l:
                    t = rand_token(rng)
                    if t not in keys:
                        keys.append(t)
                queries.append([(key, rand_token(rng)) for key in keys])
        emit(objects, queries)
        return

    if seed == 4:
        # All objects exactly 4 properties (max subset blow-up), duplicates.
        n = 50000
        m = 100000
        keys_pool = ["a", "b", "c", "d", "e"]
        vals_pool = [str(v) for v in range(5)]
        base = [make_object(rng, keys_pool, vals_pool, 4, 4) for _ in range(2000)]
        objects = [rng.choice(base) for _ in range(n)]
        queries = []
        for _ in range(m):
            obj = rng.choice(objects)
            l = rng.randint(1, 4)
            queries.append(rng.sample(obj, l))
        emit(objects, queries)
        return

    if seed == 5:
        # Exercise the "03" vs "3" value distinction explicitly.
        objects = [
            [("width", "5"), ("ht", "3")],
            [("width", "5"), ("ht", "03")],
            [("width", "05"), ("ht", "3")],
        ]
        queries = [
            [("width", "5"), ("ht", "3")],
            [("width", "5"), ("ht", "03")],
            [("width", "05")],
            [("ht", "3")],
            [("ht", "003")],
        ]
        emit(objects, queries)
        return

    # Random mid-size cases.
    n = rng.randint(1, 5000)
    m = rng.randint(1, 5000)
    pool_size = rng.randint(2, 12)
    keys_pool = list({rand_token(rng) for _ in range(pool_size)})
    if len(keys_pool) < 4:
        keys_pool = ["k1", "k2", "k3", "k4"]
    vals_pool = list({rand_token(rng, 1, 3) for _ in range(rng.randint(2, 20))})
    objects = [make_object(rng, keys_pool, vals_pool, 1, 4) for _ in range(n)]
    queries = []
    for _ in range(m):
        if objects and rng.random() < 0.5:
            obj = rng.choice(objects)
            l = rng.randint(1, len(obj))
            queries.append(rng.sample(obj, l))
        else:
            l = rng.randint(1, min(4, len(keys_pool)))
            keys = rng.sample(keys_pool, l)
            queries.append([(k, rng.choice(vals_pool)) for k in keys])
    emit(objects, queries)


if __name__ == "__main__":
    main()
