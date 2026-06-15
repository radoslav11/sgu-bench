"""Tests for p347 (Join the Strings): N <= 100 strings, each <= 100
lowercase letters; output their lexicographically smallest concatenation.

Seed 1 is a single one-letter string. Seed 2 is maximal with a comparator
stress: 100 length-100 strings drawn from {'a','b'} built as repeated
blocks, so many pairs agree on long prefixes and a+b vs b+a ties abound.
Seed 3 is maximal with prefix chains ("a", "aa", ..., and "ab"-style
prefixes), the classic trap for plain lexicographic sorting. Later seeds
mix random alphabets of size 1, 2 and 26 with assorted lengths and
duplicate strings.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
    print(rng.choice("abcdefghijklmnopqrstuvwxyz"))
    sys.exit()

lines = []
if seed == 2:
    for _ in range(100):
        block = "".join(rng.choice("ab") for _ in range(rng.randint(1, 4)))
        s = (block * 100)[:100]
        lines.append(s)
elif seed == 3:
    base = "ab" * 50
    for _ in range(100):
        k = rng.randint(1, 100)
        if rng.random() < 0.5:
            lines.append(base[:k])
        else:
            lines.append(("a" * 100)[:k])
else:
    n = rng.randint(2, 100)
    alpha = rng.choice(["a", "ab", "abc", "abcdefghijklmnopqrstuvwxyz"])
    for _ in range(n):
        if lines and rng.random() < 0.2:
            lines.append(rng.choice(lines))
        else:
            length = rng.randint(1, 100)
            lines.append("".join(rng.choice(alpha) for _ in range(length)))

print(len(lines))
print("\n".join(lines))
