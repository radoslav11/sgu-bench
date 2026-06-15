"""Tests for p182 (Open the brackets): random boolean expressions.

The input is one syntactically correct expression over a..j, length
<= 2048, with operators ! & || <=> => # and brackets. Seed 1 is the
minimal "a". Seed 2 is a near-2048-char random expression over all ten
variables; seed 3 a max-length deeply parenthesized one. Seed 4 is a
tautology over all ten variables (its full DNF is the longest possible
answer), seed 5 a contradiction. Remaining seeds are random mid-size
expressions over random variable subsets.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

OPS = ["||", "&", "<=>", "=>", "#"]
MAX_LEN = 2048


def gen_expr(rng, budget, vars, paren_bias=0.3):
    # returns a syntactically correct expression of length <= budget
    if budget < 6 or rng.random() < 0.25:
        s = rng.choice(vars)
        if budget >= 2 and rng.random() < 0.3:
            s = "!" + s
        return s
    roll = rng.random()
    if roll < 0.15:
        return "!" + gen_expr(rng, budget - 1, vars, paren_bias)
    if roll < 0.15 + paren_bias:
        return "(" + gen_expr(rng, budget - 2, vars, paren_bias) + ")"
    op = rng.choice(OPS)
    left_budget = rng.randint(1, budget - len(op) - 1)
    left = gen_expr(rng, left_budget, vars, paren_bias)
    right = gen_expr(rng, budget - len(left) - len(op), vars, paren_bias)
    # parenthesize operands of higher-priority context to stay correct
    return left + op + right


def grow_to(rng, target, vars, paren_bias=0.3):
    s = gen_expr(rng, 40, vars, paren_bias)
    while len(s) < target - 50:
        piece = gen_expr(rng, min(40, target - len(s) - 5), vars, paren_bias)
        op = rng.choice(OPS)
        if rng.random() < 0.5 and len(s) + len(piece) + len(op) + 2 <= target:
            s = "(" + s + ")" + op + piece
        elif len(s) + len(piece) + len(op) <= target:
            s = s + op + piece
        else:
            break
    return s


ALL_VARS = [chr(ord('a') + i) for i in range(10)]

if seed == 1:
    print("a")
elif seed == 2:
    print(grow_to(rng, MAX_LEN, ALL_VARS))
elif seed == 3:
    # deep nesting: many nested brackets and negations around a core
    depth = 280
    core = gen_expr(rng, MAX_LEN - 5 * depth, ALL_VARS)
    s = core
    for _ in range(depth):
        s = "(" + ("!" if rng.random() < 0.4 else "") + s + ")"
    print(s)
elif seed == 4:
    # tautology mentioning all ten variables -> maximal DNF answer
    print("||".join(f"{v}||!{v}" for v in ALL_VARS))
elif seed == 5:
    # contradiction over all ten variables
    print("&".join(f"({v}&!{v})" for v in ALL_VARS))
else:
    k = rng.randint(1, 10)
    vars = rng.sample(ALL_VARS, k)
    target = rng.randint(20, MAX_LEN)
    print(grow_to(rng, target, vars, paren_bias=rng.uniform(0.1, 0.45)))
