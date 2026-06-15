"""Tests for p337 (Keven).

Input is K (0..2000) and one cyclic string of odd length < 2000 (so up
to 1999 lowercase letters). Seed 1 is the no-answer minimum (length-1
string), seed 2 the maximal K with a maximal random string (answer is a
full-length substring, tie-broken lexicographically), seed 3 a maximal
K=0 two-letter string with many ties. Later seeds cover the all-equal
string, K=0 over the full alphabet (deep length scan), small periodic
strings with noise, and tiny strings with no valid substring.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def odd(x):
    return x if x % 2 == 1 else x - 1


if seed == 1:
    print(0)
    print("a")
elif seed == 2:
    print(2000)
    print("".join(rng.choice(string.ascii_lowercase) for _ in range(1999)))
elif seed == 3:
    print(0)
    print("".join(rng.choice("aab") for _ in range(1999)))
elif seed == 4:
    print(0)
    print("".join(rng.choice(string.ascii_lowercase) for _ in range(1999)))
elif seed == 5:
    base = "abca" * 500
    s = list(base[:1999])
    for _ in range(20):
        s[rng.randrange(1999)] = rng.choice(string.ascii_lowercase)
    print(3)
    print("".join(s))
elif seed == 6:
    print(0)
    print("z" * 1999)
elif seed == 7:
    n = odd(rng.randint(1, 25))
    print(0)
    print("".join(rng.sample(string.ascii_lowercase, n)))
else:
    n = odd(rng.randint(1, 1999))
    k = rng.randint(0, 2000)
    alpha = string.ascii_lowercase[: rng.randint(1, 26)]
    print(k)
    print("".join(rng.choice(alpha) for _ in range(n)))
