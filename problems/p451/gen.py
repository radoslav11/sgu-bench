"""Tests for p451 (Cousin's Aunt): one line "C is A('s relation)*".

At most ten relations, each one of sixteen kinship words. Seed 1 is the
empty chain ("C is A" -> 0 0). Seed 2 packs ten of the most branchy
compound relations (uncle/aunt/nephew/niece each expand to four basic
chains), maximizing the lazily built family graph and the 0-1 BFS work.
Seed 3 is ten relations drawn from the whole vocabulary. Later seeds mix
spouses, vertical chains, and the brother/sister exclusion cases.
"""

import random
import sys

RELATIONS = [
    "father", "mother", "son", "daughter", "husband", "wife",
    "brother", "sister", "grandfather", "grandmother", "grandson",
    "granddaughter", "uncle", "aunt", "nephew", "niece",
]

BRANCHY = ["uncle", "aunt", "nephew", "niece"]


def emit(rels):
    assert len(rels) <= 10
    line = "C is A"
    for r in rels:
        assert r in RELATIONS
        line += "'s " + r
    print(line)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit([])
elif seed == 2:
    emit([rng.choice(BRANCHY) for _ in range(10)])
elif seed == 3:
    emit([rng.choice(RELATIONS) for _ in range(10)])
elif seed == 4:
    # alternating up/down vertical chains exercising spouse reuse
    chain = []
    for i in range(10):
        chain.append(rng.choice(
            ["father", "mother", "son", "daughter", "husband", "wife"]))
    emit(chain)
elif seed == 5:
    # heavy on brother/sister exclusion plus aunt/uncle
    emit([rng.choice(["brother", "sister", "uncle", "aunt"])
          for _ in range(10)])
elif seed == 6:
    # grandparent / grandchild ladder
    emit([rng.choice(["grandfather", "grandmother", "grandson",
                      "granddaughter"]) for _ in range(rng.randint(6, 10))])
else:
    k = rng.randint(1, 10)
    emit([rng.choice(RELATIONS) for _ in range(k)])
