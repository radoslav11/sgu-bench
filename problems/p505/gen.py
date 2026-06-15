"""Tests for p505 (Prefixes and suffixes): count genomes having a given
prefix and a given suffix. Total genome length <= 1e5, total length of all
query prefixes plus suffixes <= 1e5.

Seed 1 is the minimum (one genome, one query). Seed 2 packs the dictionary
with length-49 "small" genomes, the case that maximises the enumerated
(prefix, suffix) hash-pair table. Seed 3 packs it with length-50 "large"
genomes and a flood of short queries, maximising the per-query scan over
large strings. Later seeds mix lengths, force many matching answers, and
add adversarial repeated structure.
"""

import random
import sys

TOTAL = 100000
LETTERS = "abcdefghijklmnopqrstuvwxyz"


def rand_str(rng, length, alphabet=LETTERS):
    k = len(alphabet)
    return "".join(alphabet[rng.randrange(k)] for _ in range(length))


def emit(genomes, queries):
    out = [str(len(genomes))]
    out.extend(genomes)
    out.append(str(len(queries)))
    for p, s in queries:
        out.append(f"{p} {s}")
    sys.stdout.write("\n".join(out) + "\n")


def build_queries(rng, genomes, budget, alphabet=LETTERS):
    """Fill queries until prefix+suffix length budget is exhausted; mix
    real (matching) and random (mostly-miss) prefix/suffix strings."""
    queries = []
    used = 0
    n = len(genomes)
    while used < budget:
        remaining = budget - used
        if remaining < 2:
            break
        if rng.random() < 0.65 and n:
            g = genomes[rng.randrange(n)]
            pl = rng.randint(1, min(len(g), max(1, remaining - 1)))
            sl = rng.randint(1, min(len(g), max(1, remaining - pl)))
            p, s = g[:pl], g[-sl:]
        else:
            pl = rng.randint(1, min(20, max(1, remaining - 1)))
            sl = rng.randint(1, min(20, max(1, remaining - pl)))
            p, s = rand_str(rng, pl, alphabet), rand_str(rng, sl, alphabet)
        queries.append((p, s))
        used += len(p) + len(s)
    return queries


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        emit(["a"], [("a", "a")])
        return

    if seed == 2:
        # All "small" genomes (length 49 < threshold 50): maximises the
        # enumerated prefix/suffix hash-pair table.
        length = 49
        cnt = TOTAL // length
        genomes = [rand_str(rng, length, "abcd") for _ in range(cnt)]
        queries = build_queries(rng, genomes, TOTAL, "abcd")
        emit(genomes, queries)
        return

    if seed == 3:
        # All "large" genomes (length 50): maximises the count of large
        # strings, each query then scans every one of them. Short queries
        # let us fit the maximum number of queries in the budget.
        length = 50
        cnt = TOTAL // length
        genomes = [rand_str(rng, length, LETTERS) for _ in range(cnt)]
        queries = []
        used = 0
        while used + 2 <= TOTAL:
            g = genomes[rng.randrange(cnt)]
            p, s = g[:1], g[-1:]
            queries.append((p, s))
            used += 2
        emit(genomes, queries)
        return

    if seed == 4:
        # One huge genome plus a few tiny ones; queries probe both.
        big = rand_str(rng, TOTAL - 100, "ab")
        genomes = [big] + [rand_str(rng, 5, "ab") for _ in range(20)]
        queries = build_queries(rng, genomes, TOTAL, "ab")
        emit(genomes, queries)
        return

    if seed == 5:
        # Many identical short genomes: every query that matches counts a
        # large, equal number of genomes.
        g = "abacaba"
        cnt = TOTAL // len(g)
        genomes = [g for _ in range(cnt)]
        queries = build_queries(rng, genomes, TOTAL, "abc")
        emit(genomes, queries)
        return

    if seed == 6:
        # Single-character genomes: maximal genome count, alphabet of 26.
        cnt = TOTAL
        genomes = [rng.choice(LETTERS) for _ in range(cnt)]
        queries = build_queries(rng, genomes, TOTAL, LETTERS)
        emit(genomes, queries)
        return

    if seed == 7:
        # Mix of small and large genomes over a tiny alphabet, so prefix
        # and suffix collisions are frequent.
        genomes = []
        used = 0
        while used < TOTAL:
            length = rng.choice([3, 10, 49, 50, 200])
            length = min(length, TOTAL - used)
            if length == 0:
                break
            genomes.append(rand_str(rng, length, "ab"))
            used += length
        queries = build_queries(rng, genomes, TOTAL, "ab")
        emit(genomes, queries)
        return

    # General randomized mid/large tests with varied alphabets.
    genomes = []
    used = 0
    alphabet = rng.choice([LETTERS, "ab", "abc", "abcde"])
    while used < TOTAL:
        length = rng.randint(1, min(300, TOTAL - used))
        genomes.append(rand_str(rng, length, alphabet))
        used += length
    queries = build_queries(rng, genomes, TOTAL, alphabet)
    emit(genomes, queries)


if __name__ == "__main__":
    main()
