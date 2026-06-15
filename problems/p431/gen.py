"""Tests for p431 (Wildcards): 1 <= n, m <= 10, names <= 11 lowercase chars.

Seed 1 is the minimum (one name to match, one to avoid). Seeds 2 and 3 are
maximal (n = m = 10, every name 11 characters long): seed 2 over a small
alphabet so names collide heavily and the backtracking search branches a
lot, seed 3 is a feasible maximal instance where all match-names share an
'a'*11 skeleton with one varying character so a real pattern must be found.
The remaining seeds mix random instances with adversarial shapes built
around a fixed base string: nested 'a' runs, suffix/prefix families, and
instances engineered to be infeasible ("OOPS").
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LOWER = string.ascii_lowercase


def emit(match, avoid):
    print(len(match), len(avoid))
    for s in match:
        print(s)
    for s in avoid:
        print(s)


def distinct_pool(count, length, alphabet):
    seen = set()
    out = []
    # bounded retries; fall back to enumerating if the alphabet is tiny
    attempts = 0
    while len(out) < count and attempts < 100000:
        attempts += 1
        s = "".join(rng.choice(alphabet) for _ in range(rng.randint(1, length)))
        if s not in seen:
            seen.add(s)
            out.append(s)
    return out


if seed == 1:
    emit(["a"], ["b"])
elif seed == 2:
    pool = []
    seen = set()
    while len(pool) < 20:
        s = "".join(rng.choice("ab") for _ in range(11))
        if s not in seen:
            seen.add(s)
            pool.append(s)
    emit(pool[:10], pool[10:])
elif seed == 3:
    # feasible maximal: every match-name has a 'z' at position 5 and avoid-names
    # never contain a 'z'; a pattern like "?????z?????" discriminates them, but
    # the search still explores the full length-11 base.
    seen = set()
    match = []
    while len(match) < 10:
        chars = [rng.choice("ab") for _ in range(11)]
        chars[5] = "z"
        s = "".join(chars)
        if s not in seen:
            seen.add(s)
            match.append(s)
    avoid = []
    while len(avoid) < 10:
        s = "".join(rng.choice("ab") for _ in range(11))
        if s not in seen:
            seen.add(s)
            avoid.append(s)
    emit(match, avoid)
elif seed == 4:
    # nested 'a' runs around single distinct letters
    match = ["a" * 11] + ["a" * 10 + c for c in "bcdefghij"]
    avoid = ["a" * k for k in range(1, 11)]
    emit(match, avoid)
elif seed == 5:
    # suffix family: match shares a common suffix, avoid lacks it
    suffix = "xyz"
    match = ["".join(rng.choice("pqrs") for _ in range(rng.randint(0, 8))) + suffix
             for _ in range(10)]
    match = list(dict.fromkeys(match))
    avoid = []
    seen = set(match)
    while len(avoid) < 10:
        s = "".join(rng.choice("pqrs") for _ in range(rng.randint(0, 8)))
        if s not in seen:
            seen.add(s)
            avoid.append(s)
    emit(match, avoid)
elif seed == 6:
    # likely infeasible: a string to match is also forbidden up to length only
    # match {ab, ba}, avoid every 2-letter string over {a,b} except those two
    match = ["ab", "ba"]
    avoid = ["aa", "bb"]
    emit(match, avoid)
else:
    n = rng.randint(1, 10)
    m = rng.randint(1, 10)
    alphabet = rng.choice(["abc", "abcd", LOWER])
    pool = distinct_pool(n + m, 11, alphabet)
    if len(pool) < n + m:
        # pad deterministically with longer distinct strings
        seen = set(pool)
        L = 1
        while len(pool) < n + m:
            cand = "".join(rng.choice(LOWER) for _ in range(min(11, 1 + L % 11)))
            if cand not in seen:
                seen.add(cand)
                pool.append(cand)
            L += 1
    emit(pool[:n], pool[n:n + m])
