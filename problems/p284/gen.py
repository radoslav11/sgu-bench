"""Tests for p284 (Grammar): N <= 30 nonterminals, |S| <= 100,
sum of description lengths <= 500, nonterminals in K's description < K.

Seed 1 is an empty grammar (answer 0), seed 2 the largest expansion
(17 copies per level, final string ~17^29 'a's, bignum answer), seed 3 a
max random grammar with a periodic pattern crossing concatenation
borders, later seeds are random grammars with empty definitions, answer-0
patterns and ab-periodic patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(s, defs):
    print(len(defs))
    print(s)
    for d in defs:
        print(len(d), *d)


def random_grammar(n, total_len):
    budget = total_len
    defs = []
    for k in range(1, n + 1):
        if budget == 0 or rng.random() < 0.1:
            defs.append([])
            continue
        L = rng.randint(1, min(budget, rng.choice([3, 8, 30])))
        budget -= L
        d = []
        for _ in range(L):
            r = rng.random()
            if r < 0.4 or k == 1:
                d.append(rng.choice("ab"))
            else:
                d.append(rng.randint(1, k - 1))
        defs.append(d)
    return defs


if seed == 1:
    emit("a", [[]])
elif seed == 2:
    # maximal expansion: 1 -> 'a'*8, k -> 17 copies of k-1; answer = 8*17^29 - 99...
    # pattern of 100 'a's, occurrences = len - 99
    defs = [["a"] * 8]
    for k in range(2, 31):
        defs.append([k - 1] * 17)
    emit("a" * 100, defs)
elif seed == 3:
    # max random grammar, periodic pattern
    defs = random_grammar(30, 500)
    emit("ab" * 50, defs)
elif seed == 4:
    # doubling of "ab" blocks, pattern "ba..ab" crossing borders
    defs = [["a", "b"] * 6]
    for k in range(2, 31):
        defs.append([k - 1, k - 1])
    emit("ba" * 40, defs)
elif seed == 5:
    # answer 0: pattern not in final string
    defs = random_grammar(30, 480)
    emit("b" * 100, defs)
else:
    n = rng.randint(1, 30)
    defs = random_grammar(n, rng.randint(0, 500))
    slen = rng.randint(1, 100)
    if seed % 2 == 0:
        s = "".join(rng.choice("ab") for _ in range(slen))
    else:
        period = "".join(rng.choice("ab") for _ in range(rng.randint(1, 4)))
        s = (period * 100)[:slen]
    emit(s, defs)
