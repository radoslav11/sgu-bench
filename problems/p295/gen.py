"""Tests for p295 (Identifier Duplicated!): 1 <= q <= c <= 63, nickname of
Latin letters, digits and spaces whose own length lies in [q, c].

Seed 1 is minimal, seed 2 maximal all-ambiguous single word, seed 3
maximal many-word nickname with q=1 (largest space-redistribution sum),
then nicknames with collapsed/multiple internal spaces, digits-only, and
random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

AMB = "ABCEHKMOPTXaceopxy"
LATIN = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
DIGITS = "0123456789"


def emit(q, c, nick):
    assert 1 <= q <= c <= 63
    assert q <= len(nick) <= c
    assert nick.strip(" ") != ""
    assert set(nick) <= set(LATIN + DIGITS + " ")
    print(q, c)
    print(nick)


if seed == 1:
    emit(1, 1, "a")
elif seed == 2:
    emit(1, 63, AMB[: 18] * 3 + AMB[:9])  # 63 ambiguous letters, one word
elif seed == 3:
    # 32 one-letter words: maximal word count, full space freedom
    emit(1, 63, " ".join("a" * 32))
elif seed == 4:
    # multiple internal spaces and short words
    emit(5, 63, "Qc  is   the  best   hacker  of  Karelia")
elif seed == 5:
    emit(3, 63, "1234567890")  # digits only: no letter variants
elif seed == 6:
    emit(63, 63, "x" * 63)  # q == c == len, no room for extra spaces
else:
    c = rng.randint(1, 63)
    q = rng.randint(1, c)
    length = rng.randint(q, c)
    alphabet = AMB + LATIN + DIGITS
    chars = [rng.choice(alphabet)]
    while len(chars) < length:
        if rng.random() < 0.15 and chars[-1] != " " and len(chars) < length - 1:
            chars.append(" ")
        else:
            chars.append(rng.choice(alphabet))
    emit(q, c, "".join(chars))
