"""Tests for p264 (Travel): stable marriage, N <= 800, names <= 10 letters.

Seed 1 is the minimal N=1 case. Seed 2 is maximal with fully random
preference permutations. Seed 3 is maximal and adversarial: all men
share one preference list while women rank men in the reverse order,
forcing a quadratic number of proposals. Later seeds are mid-size
random instances, including ones with heavily correlated preferences
(everyone chasing the same few partners).
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = string.ascii_lowercase + string.ascii_uppercase


def make_names(count):
    used = set()
    names = []
    while len(names) < 2 * count:
        ln = rng.randint(1, 10)
        name = "".join(rng.choice(LETTERS) for _ in range(ln))
        if name not in used:
            used.add(name)
            names.append(name)
    return names[:count], names[count:]


def emit(n, men, women, men_pref, women_pref):
    out = [str(n)]
    for i in range(n):
        out.append(" ".join([men[i]] + [women[j] for j in men_pref[i]]))
    for i in range(n):
        out.append(" ".join([women[i]] + [men[j] for j in women_pref[i]]))
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    print(1)
    print("Adam Eve")
    print("Eve Adam")
    sys.exit()

if seed == 2:
    n = 800
    men, women = make_names(n)
    men_pref = [rng.sample(range(n), n) for _ in range(n)]
    women_pref = [rng.sample(range(n), n) for _ in range(n)]
    emit(n, men, women, men_pref, women_pref)
elif seed == 3:
    n = 800
    men, women = make_names(n)
    shared = rng.sample(range(n), n)
    men_pref = [shared[:] for _ in range(n)]
    reverse_men = list(range(n - 1, -1, -1))
    women_pref = [reverse_men[:] for _ in range(n)]
    emit(n, men, women, men_pref, women_pref)
else:
    n = rng.randint(2, 300)
    men, women = make_names(n)
    if seed % 2 == 0:
        # correlated preferences: everyone roughly agrees who is "best"
        base_w = rng.sample(range(n), n)
        base_m = rng.sample(range(n), n)

        def jitter(base):
            lst = base[:]
            for _ in range(n // 4):
                i, j = rng.randrange(n), rng.randrange(n)
                lst[i], lst[j] = lst[j], lst[i]
            return lst

        men_pref = [jitter(base_w) for _ in range(n)]
        women_pref = [jitter(base_m) for _ in range(n)]
    else:
        men_pref = [rng.sample(range(n), n) for _ in range(n)]
        women_pref = [rng.sample(range(n), n) for _ in range(n)]
    emit(n, men, women, men_pref, women_pref)
