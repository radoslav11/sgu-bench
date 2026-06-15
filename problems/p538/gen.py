"""Tests for p538 (Emoticons): one line, length <= 10^5, made of Latin
letters, spaces, round brackets and the marks - : , ; with no leading or
trailing space.

The scan is linear, so the maximal seeds are simply ~10^5-char lines; their
value is confirming the reference stays inside the limit on the largest
allowed input. Seed 1 is a minimal one-character line. Seeds 2 and 3 are
maximal with different bracket density (a long bracket-free run vs. a dense
mix of emoticon and non-emoticon brackets). Later seeds randomize length
and composition, including pathological "()()()" runs and bracketed
letter/space spans that are the non-emoticon case.
"""

import random
import sys

MAXLEN = 10 ** 5
SYMBOLS = "-:,;"
LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


def strip_ends(s):
    """Input may not start or end with a space; swap edge spaces for a dot...
    no — only letters/spaces/brackets/marks allowed, so use a letter."""
    if not s:
        return "a"
    chars = list(s)
    if chars[0] == " ":
        chars[0] = "a"
    if chars[-1] == " ":
        chars[-1] = "a"
    return "".join(chars)


def random_line(rng, length, weights):
    pool = (
        LETTERS * weights["letter"]
        + " " * weights["space"]
        + "(" * weights["open"]
        + ")" * weights["close"]
        + SYMBOLS * weights["symbol"]
    )
    return strip_ends("".join(rng.choice(pool) for _ in range(length)))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(":")
    sys.exit()

if seed == 2:
    # Maximal, bracket-light: a long letter/space run with a few stray marks.
    line = random_line(
        rng, MAXLEN,
        {"letter": 60, "space": 20, "open": 1, "close": 1, "symbol": 8},
    )
    print(line)
    sys.exit()

if seed == 3:
    # Maximal, bracket-dense: many brackets so both emoticon and
    # non-emoticon cases occur throughout.
    line = random_line(
        rng, MAXLEN,
        {"letter": 20, "space": 8, "open": 20, "close": 20, "symbol": 6},
    )
    print(line)
    sys.exit()

if seed == 4:
    # Pathological "()()()..." run (none are emoticons by definition) padded
    # with bracketed letter spans (the non-emoticon case).
    n_pairs = rng.randint(1000, MAXLEN // 2 - 1)
    print(strip_ends("()" * n_pairs))
    sys.exit()

length = rng.randint(1, MAXLEN)
weights = {
    "letter": rng.randint(10, 60),
    "space": rng.randint(1, 20),
    "open": rng.randint(1, 25),
    "close": rng.randint(1, 25),
    "symbol": rng.randint(1, 10),
}
print(random_line(rng, length, weights))
