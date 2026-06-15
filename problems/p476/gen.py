"""Tests for p476 (Coach's Trouble).

Input: N K, then K forbidden triples a b c with 1 <= a,b,c <= 3N, all
three distinct within a triple, and all triples distinct as sets.
Constraints: 1 <= N <= 1000, 0 <= K <= 20.

Seed 1 is the minimal case N=1 K=0. Seeds 2 and 3 are maximal N=1000
K=20: seed 2 uses pairwise vertex-disjoint triples (deep
inclusion-exclusion), seed 3 uses heavily overlapping triples on a tiny
vertex set (most subsets pruned). Later seeds mix sizes and overlap
patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def distinct_triples(maxv, k):
    """Return k triples (as sorted tuples), unique as sets."""
    seen = set()
    out = []
    attempts = 0
    while len(out) < k and attempts < 100000:
        attempts += 1
        a, b, c = rng.sample(range(1, maxv + 1), 3)
        key = tuple(sorted((a, b, c)))
        if key in seen:
            continue
        seen.add(key)
        out.append((a, b, c))
    return out


def emit(n, triples):
    lines = [f"{n} {len(triples)}"]
    for a, b, c in triples:
        lines.append(f"{a} {b} {c}")
    sys.stdout.write("\n".join(lines) + "\n")


if seed == 1:
    emit(1, [])
    sys.exit()

if seed == 2:
    # Maximal: disjoint forbidden triples -> the inclusion-exclusion keeps
    # every subset (none pruned by overlap), the heaviest case.
    n, k = 1000, 20
    triples = []
    base = list(range(1, 3 * n + 1))
    rng.shuffle(base)
    for i in range(k):
        a, b, c = base[3 * i], base[3 * i + 1], base[3 * i + 2]
        triples.append((a, b, c))
    emit(n, triples)
    sys.exit()

if seed == 3:
    # Maximal N, K=20 triples drawn from a small vertex pool so many share
    # students (heavy overlap pruning).
    n, k = 1000, 20
    triples = distinct_triples(9, k)
    emit(n, triples)
    sys.exit()

if seed == 4:
    # K = 0 with maximal N: pure formula path, biggest factorials.
    emit(1000, [])
    sys.exit()

if seed == 5:
    # Small N, K at the limit drawn from all 3N students.
    n = 8
    emit(n, distinct_triples(3 * n, 20))
    sys.exit()

if seed == 6:
    # Mostly disjoint triples over a mid vertex set.
    n = 500
    emit(n, distinct_triples(3 * n, 20))
    sys.exit()

# Random mid-size tests.
n = rng.randint(1, 1000)
k = rng.randint(0, 20)
maxk_possible = (3 * n) * (3 * n - 1) * (3 * n - 2) // 6
k = min(k, maxk_possible)
emit(n, distinct_triples(3 * n, k))
