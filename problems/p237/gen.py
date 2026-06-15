"""Tests for p237 (Masters of Mind): shortest lex-smallest palindrome
from a regex of letters, '*' (any string), '!' (3 chars), '?' (1 char).

Length <= 255. Seed 2 is 255 '!' (expands to 765 wildcards, the largest
DP), seed 3 is 254 random letters plus a trailing '*' (long forced
palindrome). Other seeds: all '*', all '?', mirrored halves around '!',
pure-letter palindromes and non-palindromes, known tricky patterns
("*bab*ba", "*!ba*!"), and random special/letter mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("?")
elif seed == 2:
    print("!" * 255)
elif seed == 3:
    print("".join(rng.choice("abc") for _ in range(254)) + "*")
elif seed == 4:
    print("*" * 255)
elif seed == 5:
    print("?" * 255)
elif seed == 6:
    half = "".join(rng.choice("ab") for _ in range(126))
    print(half + "!" + half[::-1])
elif seed == 7:
    half = "".join(rng.choice("abcdefghijklmnopqrstuvwxyz")
                   for _ in range(127))
    print(half + "x" + half[::-1])  # pure-letter palindrome, no specials
elif seed == 8:
    print("".join(rng.choice("abcdefghijklmnopqrstuvwxyz")
                  for _ in range(255)))  # almost surely "NO"
elif seed == 9:
    print(rng.choice(["*bab*ba", "*!ba*!"]))
elif seed == 10:
    # palindrome skeleton with symmetric '?' pairs punched in
    half = [rng.choice("ab") for _ in range(127)]
    for _ in range(40):
        half[rng.randrange(127)] = "?"
    s = half + ["?"] + [c for c in reversed(half)]
    print("".join(s))
else:
    n = rng.randint(2, 255)
    print("".join(rng.choice("aabbc?*!") for _ in range(n)))
