"""Tests for p142 (Keyword): key-string of 'a'/'b', N <= 500000.

Seed 1 is the single-character string. Seeds 2-3 are maximal: seed 2
uniform random (answer length ~log2 N), seed 3 a de Bruijn sequence of
order 18 padded to N=500000, which contains every 18-letter word and
pushes the answer to length 19. Seeds 4-5 are degenerate (all 'a',
alternating "ab"), the rest random with skewed letter probabilities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 500000


def de_bruijn(order):
    # standard greedy (prefer-one) construction of a binary de Bruijn cycle
    seen = set()
    seq = []
    state = (0,) * order
    for _ in range(1 << order):
        for bit in (1, 0):
            nxt = state[1:] + (bit,)
            if nxt not in seen:
                seen.add(nxt)
                seq.append(bit)
                state = nxt
                break
    seq += seq[: order - 1]  # unroll the cycle into a linear string
    return "".join("ab"[b] for b in seq)


if seed == 1:
    s = "a"
elif seed == 2:
    s = "".join(rng.choice("ab") for _ in range(N_MAX))
elif seed == 3:
    s = de_bruijn(18)
    s += "".join(rng.choice("ab") for _ in range(N_MAX - len(s)))
elif seed == 4:
    s = "a" * N_MAX
elif seed == 5:
    s = "ab" * (N_MAX // 2)
else:
    n = rng.randint(1, N_MAX)
    p = rng.choice([0.05, 0.3, 0.5, 0.7, 0.95])
    s = "".join("a" if rng.random() < p else "b" for _ in range(n))

print(len(s))
print(s)
