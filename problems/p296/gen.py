"""Tests for p296 (Sasha vs. Kate): N is a decimal number with up to 1000
digits and no leading zeros; remove exactly K digits (0 <= K < |N|) to
leave the maximal number, keeping order.

The greedy is linear in |N|, so the maximal seeds use 1000-digit N with
various K. Seed 1 is minimal; seeds 2 and 3 are maximal (1000 digits) with
K near both extremes (K = 1 and K = |N| - 1). Later seeds add adversarial
digit patterns: all-equal digits, strictly increasing, strictly
decreasing, and many repeated runs, which exercise the stack popping.
"""

import random
import sys

MAXLEN = 1000
DIGITS = "0123456789"


def random_number(length, rng):
    return rng.choice("123456789") + "".join(
        rng.choice(DIGITS) for _ in range(length - 1)
    )


def emit(n, k):
    print(n, k)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit("1992", 2)
    sys.exit()

if seed == 2:
    # Maximal length, remove just one digit.
    emit(random_number(MAXLEN, rng), 1)
    sys.exit()

if seed == 3:
    # Maximal length, keep just one digit (remove all but one).
    emit(random_number(MAXLEN, rng), MAXLEN - 1)
    sys.exit()

if seed == 4:
    # Strictly increasing digits: nothing should be popped from the front;
    # the answer keeps the last len-K digits.
    body = "123456789" * (MAXLEN // 9 + 1)
    n = body[:MAXLEN]
    emit(n, rng.randint(1, MAXLEN - 1))
    sys.exit()

if seed == 5:
    # Strictly decreasing digits: the greedy keeps the prefix.
    body = "987654321" * (MAXLEN // 9 + 1)
    n = body[:MAXLEN]
    if n[0] == "0":
        n = "9" + n[1:]
    emit(n, rng.randint(1, MAXLEN - 1))
    sys.exit()

if seed == 6:
    # All equal digits: every choice ties; answer is N with K digits dropped.
    d = rng.choice("123456789")
    emit(d * MAXLEN, rng.randint(1, MAXLEN - 1))
    sys.exit()

# Random length and K, occasionally with repeated-run structure.
length = rng.randint(2, MAXLEN)
if rng.random() < 0.5:
    # Repeated runs of a few digits.
    n = rng.choice("123456789")
    while len(n) < length:
        n += rng.choice(DIGITS) * rng.randint(1, 20)
    n = n[:length]
else:
    n = random_number(length, rng)
emit(n, rng.randint(0, length - 1))
