"""Tests for p388 (Soap Opera): n <= 100 actors, two pair lists.

Actors get hidden genders and every generated pair is man-woman, so the
union graph is always bipartite (pairs in the statement are man+woman).
Pairs inside each list are distinct as unordered pairs.

Seed 1 is the minimal two-actor case, seed 2 the complete bipartite
K(50,50) in both views, seed 3 a dense random max test with a skewed
gender split, seed 4 a zero-answer case, seed 5 identical pair lists.
Remaining seeds are random sizes and densities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, juan, rosa):
    print(n, len(juan), len(rosa))
    for a, b in juan:
        print(a, b)
    for a, b in rosa:
        print(a, b)


def random_pairs(rng, men, women, count):
    all_pairs = [(a, b) for a in men for b in women]
    rng.shuffle(all_pairs)
    pairs = []
    for a, b in all_pairs[:count]:
        if rng.random() < 0.5:
            a, b = b, a
        pairs.append((a, b))
    return pairs


if seed == 1:
    emit(2, [(1, 2)], [(2, 1)])
elif seed == 2:
    men = list(range(1, 51))
    women = list(range(51, 101))
    full = [(a, b) for a in men for b in women]
    juan = full[:]
    rosa = [(b, a) for a, b in full]
    rng.shuffle(juan)
    rng.shuffle(rosa)
    emit(100, juan, rosa)
elif seed == 3:
    actors = list(range(1, 101))
    rng.shuffle(actors)
    men, women = actors[:30], actors[30:]
    emit(
        100,
        random_pairs(rng, men, women, 1800),
        random_pairs(rng, men, women, 1800),
    )
elif seed == 4:
    emit(4, [(1, 2)], [(3, 4)])
elif seed == 5:
    actors = list(range(1, 81))
    rng.shuffle(actors)
    men, women = actors[:40], actors[40:]
    pairs = random_pairs(rng, men, women, 300)
    rosa = pairs[:]
    rng.shuffle(rosa)
    emit(80, pairs, rosa)
else:
    n = rng.randint(3, 100)
    actors = list(range(1, n + 1))
    rng.shuffle(actors)
    cut = rng.randint(1, n - 1)
    men, women = actors[:cut], actors[cut:]
    cap = len(men) * len(women)
    m1 = rng.randint(1, min(cap, 2500))
    m2 = rng.randint(1, min(cap, 2500))
    emit(
        n,
        random_pairs(rng, men, women, m1),
        random_pairs(rng, men, women, m2),
    )
