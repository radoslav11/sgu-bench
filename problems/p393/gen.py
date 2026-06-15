"""Tests for p393 (Bergamot Problem): 1 <= N <= 13, 0 <= M <= 50,
distinct two-letter words over the first N letters.

Seed 1 is minimal, seed 2 a random max instance, seed 3 a max instance
where all 13 self-loops force many singleton buttons (hard for the
partition search), later seeds add a one-source star (all words share
the first letter) and random mid sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_words(n, m, forced=()):
    letters = [chr(ord('a') + i) for i in range(n)]
    words = list(forced)
    have = set(forced)
    pool = [a + b for a in letters for b in letters if a + b not in have]
    rng.shuffle(pool)
    words.extend(pool[:m - len(words)])
    return words


def emit(n, words):
    out = [f"{n} {len(words)}"]
    out.extend(words)
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, [])
elif seed == 2:
    emit(13, rand_words(13, 50))
elif seed == 3:
    loops = [chr(ord('a') + i) * 2 for i in range(13)]
    emit(13, rand_words(13, 50, forced=loops))
elif seed == 4:
    emit(13, [])
elif seed == 5:
    star = ['a' + chr(ord('a') + i) for i in range(13)]
    emit(13, rand_words(13, 50, forced=star))
elif seed == 6:
    # Dense conflicts on a small alphabet: many of the 36 pairs used.
    emit(6, rand_words(6, 30))
elif seed == 7:
    emit(1, ["aa"])
else:
    n = rng.randint(1, 13)
    m = rng.randint(0, min(50, n * n))
    emit(n, rand_words(n, m))
