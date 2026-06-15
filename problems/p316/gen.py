"""Tests for p316 (Code Tanks): 2 <= N <= 6 tanks, 1 <= M <= 4000 shots.
Each shot is (shooter, target), shooter != target, and the shooter must be
"active" (hp > 0) at the moment it fires.

Seed 1 is the minimal case (N=2, M=1). Seeds 2 and 3 are maximal (N=6,
M=4000): seed 2 spreads damage uniformly so many tanks stay alive, seed 3
concentrates fire to kill tanks early and exercise the "target already
dead -> no score" branch heavily. The rest are randomized small/medium
cases plus all-on-one and round-robin adversarial patterns.

Every emitted shot respects the active-shooter rule, so the log is always
a legal match.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, shots):
    out = [f"{n} {len(shots)}"]
    out.extend(f"{s} {t}" for s, t in shots)
    sys.stdout.write("\n".join(out) + "\n")


def random_pick(hp, alive):
    s = rng.choice(alive)
    # target: any other tank (alive or already dead is allowed)
    t = rng.randrange(len(hp))
    while t == s:
        t = rng.randrange(len(hp))
    return s, t


def gen_random(n, m, pick=random_pick):
    hp = [100] * n
    shots = []
    for _ in range(m):
        alive = [i for i in range(n) if hp[i] > 0]
        if not alive:
            break
        s, t = pick(hp, alive)
        hp[t] -= 8
        shots.append((s + 1, t + 1))
    return shots


if seed == 1:
    emit(2, [(1, 2)])
elif seed == 2:
    # N=6, spread damage so tanks survive long: round-robin targets.
    n = 6
    hp = [100] * n
    shots = []
    counter = 0
    while len(shots) < 4000:
        alive = [i for i in range(n) if hp[i] > 0]
        if not alive:
            break
        s = alive[counter % len(alive)]
        # target the highest-hp other tank to keep many alive
        cand = [i for i in range(n) if i != s]
        t = max(cand, key=lambda i: hp[i])
        hp[t] -= 8
        shots.append((s + 1, t + 1))
        counter += 1
    emit(n, shots)
elif seed == 3:
    # N=6, focus fire: kill tanks one by one, lots of shots wasted on dead
    # targets so the "no score for hitting dead tank" path dominates.
    n = 6
    hp = [100] * n
    shots = []
    while len(shots) < 4000:
        alive = [i for i in range(n) if hp[i] > 0]
        if not alive:
            break
        s = alive[0]
        # target the last tank index (gets killed, then keep hitting it)
        t = n - 1 if s != n - 1 else 0
        hp[t] -= 8
        shots.append((s + 1, t + 1))
    emit(n, shots)
elif seed == 4:
    # all tanks hammer tank 1
    n = rng.randint(2, 6)
    hp = [100] * n
    shots = []
    while len(shots) < rng.randint(50, 4000):
        alive = [i for i in range(n) if hp[i] > 0]
        alive_other = [i for i in alive if i != 0]
        if not alive_other:
            break
        s = rng.choice(alive_other)
        hp[0] -= 8
        shots.append((s + 1, 1))
    emit(n, shots)
elif seed == 5:
    n = rng.randint(2, 6)
    emit(n, gen_random(n, rng.randint(1, 4000)))
elif seed == 6:
    n = rng.randint(2, 6)
    emit(n, gen_random(n, rng.randint(1, 200)))
elif seed == 7:
    # N=6 maximal length random
    emit(6, gen_random(6, 4000))
else:
    n = rng.randint(2, 6)
    emit(n, gen_random(n, rng.randint(1, 4000)))
