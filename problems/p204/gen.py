"""Tests for p204 (Little Jumper): lines of eight reals b1 t1 b2 t2 l ds df g.

All values lie in [1e-2, 1e3] with t1 >= b1 + 1e-2 and t2 >= b2 + 1e-2.
Seed 1 is a single minimal-magnitude case, seeds 2-3 are 20000-case files
(uniform vs log-scale extreme values), the rest mix the two samples with
random cases including narrow holes and tiny/huge gravity.
"""

import random
import sys

LO, HI = 0.01, 1000.0

seed = int(sys.argv[1])
rng = random.Random(seed)


def fmt(x):
    return f"{x:.4f}"


def rand_uniform():
    return rng.uniform(LO, HI)


def rand_log():
    import math

    return math.exp(rng.uniform(math.log(LO), math.log(HI)))


def make_case(value):
    b1 = value()
    t1 = min(HI, b1 + 0.01 + abs(value()) * rng.random())
    b2 = value()
    t2 = min(HI, b2 + 0.01 + abs(value()) * rng.random())
    l, ds, df, g = value(), value(), value(), value()
    return " ".join(fmt(x) for x in (b1, t1, b2, t2, l, ds, df, g))


if seed == 1:
    print(" ".join(fmt(x) for x in (0.01, 0.02, 0.01, 0.02, 0.01, 0.01, 0.01, 0.01)))
    sys.exit()

if seed == 2:
    cases = [make_case(rand_uniform) for _ in range(20000)]
elif seed == 3:
    cases = [make_case(rand_log) for _ in range(20000)]
else:
    cases = []
    cases.append("0.3 1.0 0.5 0.9 1.7 1.2 2.3 9.8")
    cases.append("0.6 0.8 0.6 0.8 2.4 0.3 1.5 0.7")
    for _ in range(rng.randint(50, 500)):
        cases.append(make_case(rng.choice([rand_uniform, rand_log])))

print("\n".join(cases))
