"""Tests for p506 (Subsequences Of Substrings): count ways to trim a
prefix and a suffix of the text so the message stays a subsequence. Text
length up to 1e6, message length up to 100, and the message is guaranteed
to be a subsequence of the text.

Seed 1 is the minimum (text and message both one letter). Seeds 2 and 3
use a full 1e6 text with a length-100 message: seed 2 spreads the message
characters evenly so every start position must scan deep before matching,
seed 3 uses a single-letter alphabet so the message matches almost
immediately from every start. Later seeds vary alphabet size, message
placement and degenerate shapes (message equal to the whole text, message
clustered at one end).
"""

import random
import sys

NMAX = 1000000
MMAX = 100
LETTERS = "abcdefghijklmnopqrstuvwxyz"


def emit(text, msg):
    sys.stdout.write(text + "\n" + msg + "\n")


def pick_subsequence(rng, text, m):
    """Pick m positions in increasing order to form a guaranteed subseq."""
    n = len(text)
    m = min(m, n)
    idx = sorted(rng.sample(range(n), m))
    return "".join(text[i] for i in idx)


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        emit("a", "a")
        return

    if seed == 2:
        # Full text over the whole alphabet, message of length 100 whose
        # characters are spread across the text so matching from every
        # start has to walk a long way.
        text = "".join(LETTERS[rng.randrange(26)] for _ in range(NMAX))
        # Take characters at evenly spaced positions.
        step = NMAX // MMAX
        idx = [i * step + rng.randrange(step) for i in range(MMAX)]
        msg = "".join(text[i] for i in idx)
        emit(text, msg)
        return

    if seed == 3:
        # Single-letter text and message: every start matches at once, but
        # the O(n*m) loop still runs n*m character jumps.
        text = "a" * NMAX
        emit(text, "a" * MMAX)
        return

    if seed == 4:
        # Two-letter text: short next-occurrence jumps, message length 100.
        text = "".join(rng.choice("ab") for _ in range(NMAX))
        msg = pick_subsequence(rng, text, MMAX)
        emit(text, msg)
        return

    if seed == 5:
        # Message is the entire (short) text.
        n = rng.randint(1, MMAX)
        text = "".join(LETTERS[rng.randrange(3)] for _ in range(n))
        emit(text, text)
        return

    if seed == 6:
        # Message characters all clustered at the very end of the text, so
        # almost every start must scan to the tail.
        text = "".join(rng.choice("abcde") for _ in range(NMAX))
        tail = text[-MMAX:]
        emit(text, tail)
        return

    if seed == 7:
        # Message characters all clustered at the very beginning.
        text = "".join(rng.choice("abcde") for _ in range(NMAX))
        msg = text[:MMAX]
        emit(text, msg)
        return

    if seed == 8:
        # Mid-size random text, random subsequence message.
        n = rng.randint(1000, 50000)
        text = "".join(LETTERS[rng.randrange(26)] for _ in range(n))
        m = rng.randint(1, min(MMAX, n))
        emit(text, pick_subsequence(rng, text, m))
        return

    # General randomized tests with varied size and alphabet.
    n = rng.randint(1, NMAX)
    alphabet = rng.choice([LETTERS, "ab", "abc", "abcde"])
    text = "".join(alphabet[rng.randrange(len(alphabet))] for _ in range(n))
    m = rng.randint(1, min(MMAX, n))
    emit(text, pick_subsequence(rng, text, m))


if __name__ == "__main__":
    main()
