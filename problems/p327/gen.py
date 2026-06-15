"""Tests for p327 (Yet Another Palindrome): 1 <= N <= 14 lowercase words,
each 1..30 characters.

Seed 1 is the single one-letter word, seed 2 has 14 random 30-char words
over {a,b} (overlap-heavy), seed 3 has 14 max-length words sharing a
common core plus reversed twins. Later seeds cover duplicates, words
contained in other words, palindromic words and random alphabets.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def word(length, alphabet):
    return "".join(rng.choice(alphabet) for _ in range(length))


def emit(words):
    print(len(words))
    for w in words:
        print(w)


if seed == 1:
    emit(["a"])
elif seed == 2:
    emit([word(30, "ab") for _ in range(14)])
elif seed == 3:
    core = word(12, "abc")
    words = []
    for i in range(7):
        w = word(9, "abc") + core + word(9, "abc")
        words.append(w)
        words.append(w[::-1])
    emit(words)
elif seed == 4:
    # Duplicates and substrings of longer words.
    big = word(30, "ab")
    words = [big, big, big[3:20], big[5:9][::-1], big[::-1], "a", "b"]
    rng.shuffle(words)
    emit(words)
elif seed == 5:
    emit(["abacaba"] * 14)
elif seed == 6:
    # Palindromic words of various lengths.
    words = []
    for _ in range(10):
        half = word(rng.randint(1, 14), "ab")
        mid = rng.choice(["", "c"])
        words.append((half + mid + half[::-1])[:30])
    emit(words)
elif seed == 7:
    # Pairs that are exact reverses of each other.
    words = []
    for _ in range(7):
        w = word(rng.randint(5, 30), "abc")
        words.append(w)
        words.append(w[::-1])
    emit(words)
elif seed == 8:
    # Chain words with planted suffix-prefix overlaps.
    pieces = [word(6, "ab") for _ in range(15)]
    words = [pieces[i] + pieces[i + 1] for i in range(14)]
    emit(words)
else:
    n = rng.randint(1, 14)
    alphabet = rng.choice(["ab", "abc", "abcdefghijklmnopqrstuvwxyz"])
    emit([word(rng.randint(1, 30), alphabet) for _ in range(n)])
