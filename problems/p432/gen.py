"""Tests for p432 (XYZX 2009): insert "not"/"no" negations into a text.

Input is a single line of sentences (latin letters, single spaces, dots),
at most 1024 bytes. Seed 1 is the minimal text, seeds 2-3 are full
1024-byte texts: seed 2 packed with trigger words (maximal output),
seed 3 mixed-case prose with articles after is/are. Later seeds hit
edge patterns: triggers as last word, articles at sentence end, words
like Not or No already present, and texts with zero triggers.
"""

import random
import sys

LIMIT = 1024

TRIGGERS = ["can", "may", "must", "should", "is", "are"]
ARTICLES = ["a", "the"]
FILLERS = [
    "field", "label", "rectangle", "particle", "collision", "output",
    "symbol", "expression", "string", "problem", "statement", "Misha",
    "blank", "line", "test", "case", "different", "ways", "viewed",
    "equal", "understood", "correctly", "given", "each", "other",
    "not", "no", "isle", "cane", "mayor", "musty", "are", "His",
]


def vary_case(rng, w):
    r = rng.random()
    if r < 0.6:
        return w
    if r < 0.8:
        return w.capitalize()
    if r < 0.9:
        return w.upper()
    return "".join(c.upper() if rng.random() < 0.5 else c.lower() for c in w)


def make_sentence(rng, trigger_prob, max_words):
    n = rng.randint(1, max_words)
    words = []
    for i in range(n):
        r = rng.random()
        if r < trigger_prob:
            words.append(vary_case(rng, rng.choice(TRIGGERS)))
            if rng.random() < 0.5 and i + 1 < n:
                words.append(vary_case(rng, rng.choice(ARTICLES)))
        elif r < trigger_prob + 0.1:
            words.append(vary_case(rng, rng.choice(ARTICLES)))
        else:
            words.append(rng.choice(FILLERS))
    return " ".join(words) + "."


def fill_text(rng, trigger_prob, max_words):
    text = make_sentence(rng, trigger_prob, max_words)
    while True:
        s = make_sentence(rng, trigger_prob, max_words)
        if len(text) + 1 + len(s) + 1 > LIMIT:
            break
        text += " " + s
    return text


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("Is.")
elif seed == 2:
    print(fill_text(rng, 0.75, 30))
elif seed == 3:
    print(fill_text(rng, 0.25, 60))
elif seed == 4:
    sentences = [
        "It is.",
        "It is a.",
        "They are the.",
        "You can.",
        "Maybe you may.",
        "He must.",
        "She should.",
        "There IS the field.",
        "There ARE a fields.",
        "CAN MAY MUST SHOULD IS ARE.",
        "is a the is.",
        "He is not here.",
        "No one can no.",
    ]
    print(" ".join(sentences))
elif seed == 5:
    print("Nothing interesting happens here. Just plain words without "
          "any trigger at all. Cane mayor musty isle mistress.")
else:
    print(fill_text(rng, rng.uniform(0.1, 0.5), rng.randint(3, 40)))
