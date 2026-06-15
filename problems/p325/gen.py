"""Tests for p325 (Palindrome): word of capital letters, 1 <= |s| <= 1000000.

Seed 1 is the single-letter minimum, seeds 2-3 are maximal-length words
(random feasible binary word; interleaved block pattern with many
inversions). Later seeds mix feasible words, impossible words (-1),
wide alphabets and already-palindromic inputs.
"""

import random
import sys

MAXN = 1000000

seed = int(sys.argv[1])
rng = random.Random(seed)


def feasible_word(n, alphabet):
    """Random word of length n that CAN be rearranged into a palindrome."""
    letters = []
    remaining = n
    if n % 2 == 1:
        letters.append(rng.choice(alphabet))
        remaining -= 1
    while remaining > 0:
        c = rng.choice(alphabet)
        letters.append(c)
        letters.append(c)
        remaining -= 2
    rng.shuffle(letters)
    return "".join(letters)


if seed == 1:
    print("A")
elif seed == 2:
    print(feasible_word(MAXN, "AB"))
elif seed == 3:
    # Interleaved blocks ABAB...AB repeated: forces a huge inversion count.
    block = "AB" * 500
    word = (block + block[::-1]) * (MAXN // (4 * 500))
    print(word[:MAXN])
elif seed == 4:
    # Maximal-length word over the full alphabet, likely infeasible (-1).
    print("".join(rng.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(MAXN)))
elif seed == 5:
    print("Z" * MAXN)
elif seed == 6:
    n = rng.randint(2, 30)
    print(feasible_word(n, "ABC"))
elif seed == 7:
    print(feasible_word(rng.randint(1000, 5000), "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
elif seed == 8:
    # Guaranteed impossible: two characters with odd counts.
    n = rng.randint(10, 1000) * 2
    word = list(feasible_word(n, "ABCD"))
    word[0] = "Y"
    word[1] = "Z"
    print("".join(word))
elif seed == 9:
    # Already a palindrome: answer 0.
    half = "".join(rng.choice("ABC") for _ in range(rng.randint(1, 200000)))
    mid = rng.choice(["", rng.choice("ABC")])
    print(half + mid + half[::-1])
else:
    print(feasible_word(rng.randint(100000, 400000), "ABCDE"))
