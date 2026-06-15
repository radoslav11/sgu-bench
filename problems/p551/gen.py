"""Tests for p551 (Preparing Problem): 1 <= n, t1, t2 <= 5000.

The input is a single line of three small integers, so the seed plan
walks the corners and then fills in random and adversarial values.

- seed 1: minimal input (n = t1 = t2 = 1).

- seed 2: maximal values (n = t1 = t2 = 5000).

- seed 3: maximal n with very different speeds (t1 = 1, t2 = 5000) so the
  binary-search range is widest and the two-pending case is exercised.

- seeds 4+: random values, plus equal-speed ties and one-much-faster
  layouts that stress the pending-job bookkeeping.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 5000


def emit(n, t1, t2):
    sys.stdout.write(f"{n} {t1} {t2}\n")


if seed == 1:
    emit(1, 1, 1)

elif seed == 2:
    emit(LIM, LIM, LIM)

elif seed == 3:
    emit(LIM, 1, LIM)

elif seed == 4:
    emit(LIM, LIM, 1)

elif seed == 5:
    # Equal speeds, large n -> heavy tie at the binary-search boundary.
    t = rng.randint(1, LIM)
    emit(LIM, t, t)

elif seed == 6:
    # n = 1: stops almost immediately.
    emit(1, rng.randint(1, LIM), rng.randint(1, LIM))

elif seed == 7:
    # Coprime-ish speeds to break alignment of finishing times.
    emit(rng.randint(1, LIM), 7, 11)

else:
    emit(rng.randint(1, LIM), rng.randint(1, LIM), rng.randint(1, LIM))
