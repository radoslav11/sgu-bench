"""Tests for p494 (Journal): typeset N words of width W with an R x C image.

The input is T test cases, each "N W R C" followed by N word lengths
(1 <= L_i <= W, 1 <= C <= W). Words are placed greedily; the image is
positioned to minimise used rows. The cost of the reference grows with
W * N, so seeds 2 and 3 push N and W to their joint maximum with two
different word-length distributions.

Bounds used here: N <= 50000, W <= 1000, R <= 1000, sum of N <= 50000.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 50000
W_MAX = 1000
R_MAX = 1000


def emit(cases):
    out = [str(len(cases))]
    for n, w, r, c, lens in cases:
        out.append(f"{n} {w} {r} {c}")
        out.append(" ".join(map(str, lens)))
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    # Minimal valid case: one word, width 1, image 1x1.
    emit([(1, 1, 1, 1, [1])])

elif seed == 2:
    # Maximal: N and W at the limit, image as tall as allowed and one
    # column wide so almost the whole width stays usable. Uniform word
    # lengths over the full width.
    w = W_MAX
    lens = [rng.randint(1, w) for _ in range(N_MAX)]
    emit([(N_MAX, w, R_MAX, 1, lens)])

elif seed == 3:
    # Second maximal: same N, W and R but a bimodal length distribution
    # (a mix of short and near-full-width words). C is kept at 1 so both
    # free segments are large, which keeps the column loop heavy.
    w = W_MAX
    lens = [
        rng.randint(1, w // 4) if rng.random() < 0.5 else rng.randint(3 * w // 4, w)
        for _ in range(N_MAX)
    ]
    emit([(N_MAX, w, R_MAX, 1, lens)])

elif seed == 4:
    # Many small test cases, total N bounded, varied small dimensions.
    cases = []
    total = 0
    while total < N_MAX - 50:
        n = rng.randint(1, 200)
        if total + n > N_MAX:
            n = N_MAX - total
        total += n
        w = rng.randint(1, 50)
        r = rng.randint(1, 30)
        c = rng.randint(1, w)
        lens = [rng.randint(1, w) for _ in range(n)]
        cases.append((n, w, r, c, lens))
    emit(cases)

elif seed == 5:
    # All words equal to the full width: every word forces its own row,
    # image can only shift things by full rows.
    w = rng.randint(2, W_MAX)
    n = rng.randint(1000, 20000)
    lens = [w] * n
    emit([(n, w, rng.randint(1, R_MAX), rng.randint(1, w), lens)])

elif seed == 6:
    # Wide image that nearly fills the width (C close to W), leaving thin
    # free segments; long words may not fit beside it at all.
    w = rng.randint(50, W_MAX)
    n = rng.randint(1000, 30000)
    c = rng.randint(max(1, w - 3), w)
    lens = [rng.randint(1, w) for _ in range(n)]
    emit([(n, w, rng.randint(1, R_MAX), c, lens)])

elif seed == 7:
    # Long words around half the width plus the spacing rule, classic
    # word-wrap stress with a moderate image.
    w = rng.randint(100, W_MAX)
    n = rng.randint(5000, 40000)
    lens = [rng.randint(w // 2, w) for _ in range(n)]
    emit([(n, w, rng.randint(1, R_MAX), rng.randint(1, w), lens)])

else:
    # General randomised mid-size test.
    w = rng.randint(1, W_MAX)
    n = rng.randint(1, min(N_MAX, 30000))
    c = rng.randint(1, w)
    r = rng.randint(1, R_MAX)
    lens = [rng.randint(1, w) for _ in range(n)]
    emit([(n, w, r, c, lens)])
