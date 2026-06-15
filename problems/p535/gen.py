"""Tests for p535 (Dirty Dishes): one line, <= 2500 chars, at most five
asterisks, at least one letter.

The interval DP work is heaviest when the post-reduction string stays long
(few forced cancellations) and carries stars that must balance it, so the
maximal seeds use long all-lowercase strings with the five stars sprinkled
in. Seed 1 is the minimal one-letter input. Seeds 2 and 3 are maximal
(2500 chars) with different letter and star layouts. Later seeds mix random
push/pop sequences, balanced bracket strings, contradictions, and
star-heavy fragments.
"""

import random
import sys

MAXLEN = 2500
MAXSTAR = 5

seed = int(sys.argv[1])
rng = random.Random(seed)


def with_stars(chars, nstars):
    chars = list(chars)
    for _ in range(nstars):
        chars.insert(rng.randint(0, len(chars)), "*")
    return "".join(chars)


if seed == 1:
    # Minimal valid input: a single letter.
    print("a")
    sys.exit()

if seed == 2:
    # Maximal: 2495 identical lowercase pushes plus 5 stars.
    print(with_stars("a" * (MAXLEN - MAXSTAR), MAXSTAR))
    sys.exit()

if seed == 3:
    # Maximal, different structure: 2495 lowercase letters drawn from a few
    # colors so reductions are sparse, plus 5 stars.
    palette = "abcde"
    body = "".join(rng.choice(palette) for _ in range(MAXLEN - MAXSTAR))
    print(with_stars(body, MAXSTAR))
    sys.exit()

if seed == 4:
    # Maximal balanced bracket string (each push immediately popped) so the
    # greedy reduction empties it; stresses the reduction path.
    pairs = []
    for _ in range((MAXLEN - 4) // 2):
        c = rng.choice("abcdefgh")
        pairs.append(c + c.upper())
    s = "".join(pairs)
    print(with_stars(s, rng.randint(0, MAXSTAR)))
    sys.exit()

# Randomized mixed cases of moderate-to-large size.
length = rng.randint(1, MAXLEN - MAXSTAR)
mode = rng.choice(["pushpop", "lowerheavy", "nested", "messy"])

if mode == "pushpop":
    # Simulate a real stack to often produce solvable strings.
    stack = []
    chars = []
    for _ in range(length):
        if stack and rng.random() < 0.5:
            chars.append(stack.pop().upper())
        else:
            c = rng.choice("abc")
            stack.append(c)
            chars.append(c)
    body = "".join(chars)
elif mode == "lowerheavy":
    body = "".join(rng.choice("ab") if rng.random() < 0.8 else rng.choice("AB")
                    for _ in range(length))
elif mode == "nested":
    colors = "abcdefghij"
    half = length // 2
    opens = [rng.choice(colors) for _ in range(half)]
    body = "".join(opens) + "".join(c.upper() for c in reversed(opens))
else:  # messy: fully random letters, likely contradictions
    body = "".join(
        rng.choice("aAbBcC") for _ in range(length)
    )

nstars = rng.randint(0, MAXSTAR)
out = with_stars(body, nstars)
if not any(ch.isalpha() for ch in out):
    out = "a" + out
if len(out) > MAXLEN:
    out = out[:MAXLEN]
print(out)
