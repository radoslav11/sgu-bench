"""Tests for p411 (Petya the Hero): two strings, <= 2000 lowercase each.

The strings must share at least one letter. Seed 1 is minimal, seed 2 a
maximal binary-alphabet pair (palindromes everywhere), seed 3 maximal
single-letter strings (answer length 2000), later seeds plant long
palindromes inside random noise or use wide alphabets.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_str(n, alpha):
    return "".join(rng.choice(alpha) for _ in range(n))


def ensure_common(a, b):
    if set(a) & set(b):
        return a, b
    return a, b[:-1] + a[0]


if seed == 1:
    a, b = "a", "a"
elif seed == 2:
    a, b = rand_str(2000, "ab"), rand_str(2000, "ab")
elif seed == 3:
    a, b = "a" * 2000, "a" * 2000
elif seed == 4:
    # long planted palindrome inside wide-alphabet noise
    half = rand_str(600, string.ascii_lowercase)
    pal = half + half[::-1]
    a = rand_str(400, string.ascii_lowercase) + pal + rand_str(400, string.ascii_lowercase)
    b = rand_str(300, string.ascii_lowercase) + pal + rand_str(500, string.ascii_lowercase)
elif seed == 5:
    # odd palindrome planted, plus a longer palindrome present in only one string
    half = rand_str(500, "abc")
    pal = half + "z" + half[::-1]
    decoy = "y" * 1500
    a = decoy + pal
    b = pal + rand_str(700, "abc")
elif seed == 6:
    a = rand_str(2000, string.ascii_lowercase)
    b = a
elif seed == 7:
    # almost-equal single letter with rare separators (huge palindromes, ties)
    a = "".join("b" if rng.random() < 0.01 else "a" for _ in range(2000))
    b = "".join("c" if rng.random() < 0.01 else "a" for _ in range(2000))
else:
    n = rng.randint(1, 2000)
    m = rng.randint(1, 2000)
    alpha = string.ascii_lowercase[: rng.choice([2, 3, 5, 26])]
    a, b = rand_str(n, alpha), rand_str(m, alpha)

a, b = ensure_common(a, b)
print(a)
print(b)
