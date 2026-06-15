"""Tests for p508 (Black-white balls): given n, l1, l2, p, output the
shortest confidence interval [a, b] (smallest a on ties). Constraints:
1 <= n <= 50, 0 <= l1 <= n, 0 <= l2 <= n - l1, 0 <= p <= 100.

The state space is tiny, so the seeds focus on covering it: seed 1 is the
n=1 minimum, seeds 2 and 3 use n=50 (the maximum) with extreme and
boundary confidence levels, and later seeds sweep edge draws (no black, no
white, all one colour), p=0/p=100, and randomized mid cases.
"""

import random
import sys

NMAX = 50


def emit(n, l1, l2, p):
    sys.stdout.write(f"{n} {l1} {l2} {p}\n")


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        # Minimum n; no balls drawn, full confidence required.
        emit(1, 0, 0, 100)
        return

    if seed == 2:
        # Maximal n with full confidence and a lopsided draw.
        emit(NMAX, 1, 24, 100)
        return

    if seed == 3:
        # Maximal n, 95% confidence, small sample -> wide interval.
        emit(NMAX, 1, 10, 95)
        return

    if seed == 4:
        # p = 0: any interval qualifies, expect the [0, 0] single point.
        emit(NMAX, rng.randint(0, NMAX), 0, 0)
        return

    if seed == 5:
        # No balls drawn at all, max n, high but not full confidence.
        emit(NMAX, 0, 0, 90)
        return

    if seed == 6:
        # All drawn balls black (l2 = 0), full confidence.
        l1 = rng.randint(1, NMAX)
        emit(NMAX, l1, 0, 100)
        return

    if seed == 7:
        # All drawn balls white (l1 = 0), full confidence.
        l2 = rng.randint(1, NMAX)
        emit(NMAX, 0, l2, 100)
        return

    if seed == 8:
        # Entire bag drawn (l1 + l2 = n): k is determined exactly.
        n = NMAX
        l1 = rng.randint(0, n)
        emit(n, l1, n - l1, rng.randint(0, 100))
        return

    if seed == 9:
        # Mid n, p near a tie boundary value.
        n = rng.randint(10, NMAX)
        l1 = rng.randint(0, n)
        l2 = rng.randint(0, n - l1)
        emit(n, l1, l2, rng.choice([50, 68, 95, 99]))
        return

    # General randomized cases over the whole space.
    n = rng.randint(1, NMAX)
    l1 = rng.randint(0, n)
    l2 = rng.randint(0, n - l1)
    p = rng.randint(0, 100)
    emit(n, l1, l2, p)


if __name__ == "__main__":
    main()
