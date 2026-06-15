"""Tests for p161 (validity of formulae in the Heyting algebra of a poset).

Constraints: N <= 100 vertices, M <= 5000 edges, K <= 20 formulae, line
length <= 254, D = |H| <= 100 antichains, Sum(D^vars) <= 10^6 total
assignment work. Since any poset on N elements has at least N + 1 order
ideals, D <= 100 forces N <= 99, with N = 99 only as a chain.

The generator enumerates the ideals of every candidate DAG itself
(rejecting D > 100) and assigns each formula a variable count that fits
the remaining D^v budget. Formulae mix random expression trees with a
bank of intuitionistic tautologies and classical-only laws, with random
extra spaces between tokens.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = [chr(ord("A") + i) for i in range(26)]


def ideal_count(n, edges, cap=101):
    up = [1 << i for i in range(n)]
    for s, t in edges:
        up[s] |= 1 << t
    for k in range(n):
        bk = 1 << k
        for i in range(n):
            if up[i] & bk:
                up[i] |= up[k]
    down = [0] * n
    for x in range(n):
        for z in range(n):
            if up[z] >> x & 1:
                down[x] |= 1 << z
    full = (1 << n) - 1
    seen = {0}
    queue = [0]
    front = 0
    while front < len(queue):
        cur = queue[front]
        front += 1
        for x in range(n):
            if cur >> x & 1:
                continue
            if bin(down[x] & (full ^ cur)).count("1") != 1:
                continue
            nxt = cur | (1 << x)
            if nxt not in seen:
                seen.add(nxt)
                queue.append(nxt)
                if len(seen) > cap:
                    return cap + 1
    return len(seen)


def chain_edges(order, extra_closure=0, rng=None):
    # order: list of vertex ids forming a chain; optionally add random
    # transitive-closure edges on top of the covers.
    edges = [(order[i], order[i + 1]) for i in range(len(order) - 1)]
    if extra_closure and rng:
        closure = [
            (order[i], order[j])
            for i in range(len(order))
            for j in range(i + 2, len(order))
        ]
        rng.shuffle(closure)
        edges += closure[:extra_closure]
    return edges


def random_formula(rng, var_pool, depth):
    if depth == 0 or (depth < 3 and rng.random() < 0.3):
        return rng.choice(var_pool + ["0", "1"] if var_pool else ["0", "1"])
    op = rng.choice(["~", "&", "|", "=>", "=", "&", "|", "=>"])
    if op == "~":
        return "~" + random_formula(rng, var_pool, depth - 1)
    a = random_formula(rng, var_pool, depth - 1)
    b = random_formula(rng, var_pool, depth - 1)
    return "(" + a + op + b + ")"


BANK = [
    # (formula template, number of distinct variables)
    ("{0}=>{0}", 1),
    ("{0}=>{1}=>{0}", 2),
    ("{0}=>{1}=>{2} = ({0}&{1})=>{2}", 3),
    ("{0} => ~~{0}", 1),
    ("~{0} = ~~~{0}", 1),
    ("{0}&({1}|{2}) = {0}&{1}|{0}&{2}", 3),
    ("({0}=>{2})&({1}=>{2}) => {0}|{1}=>{2}", 3),
    ("~({0}|{1}) = ~{0}&~{1}", 2),
    ("0=>{0}", 1),
    ("{0}&{1}=>{0}", 2),
    ("{0}=>{0}|{1}", 2),
    ("{0}|~{0}", 1),
    ("~~{0} => {0}", 1),
    ("(({0}=>{1})=>{0})=>{0}", 2),
    ("({0}=>{1})|({1}=>{0})", 2),
    ("~({0}&{1}) = ~{0}|~{1}", 2),
    ("{0} = ~~{0}", 1),
    ("({0} => {1}) = ({1} | ~{0})", 2),
    ("1=0", 0),
    ("1", 0),
]


def spaced(formula, rng):
    out = []
    i = 0
    while i < len(formula):
        if formula[i] == "=" and i + 1 < len(formula) and formula[i + 1] == ">":
            out.append("=>")
            i += 2
        else:
            out.append(formula[i])
            i += 1
        if rng.random() < 0.15:
            out.append(" " * rng.randint(1, 3))
    s = "".join(out).rstrip()
    return s


def make_formulas(rng, d, k, heavy_first=False):
    budget = 10**6 - k  # reserve k so trailing 0-var formulas never overflow
    lines = []
    if heavy_first and d**3 <= budget:
        # a valid 3-variable formula evaluates all d^3 assignments
        names = rng.sample(LETTERS, 3)
        lines.append(spaced("{0}=>{1}=>{2} = ({0}&{1})=>{2}".format(*names), rng))
        budget -= d**3
        k -= 1
    for _ in range(k):
        max_v = 0
        while max_v < 6 and d ** (max_v + 1) <= budget:
            max_v += 1
        if rng.random() < 0.5:
            tpl, nv = rng.choice([x for x in BANK if x[1] <= max_v])
            names = rng.sample(LETTERS, nv) if nv else []
            f = tpl.format(*names)
        else:
            nv = rng.randint(0, max_v)
            pool = rng.sample(LETTERS, nv)
            while True:
                f = random_formula(rng, pool, rng.randint(1, 4))
                if len(f) <= 200:
                    break
            nv = len(set(c for c in f if "A" <= c <= "Z"))
        budget -= d**nv
        f = spaced(f, rng)
        if len(f) > 254:
            f = f.replace(" ", "")
        lines.append(f)
    return lines


def emit(n, edges, formulas):
    print(n, len(edges))
    for s, t in edges:
        print(s + 1, t + 1)
    print(len(formulas))
    for f in formulas:
        print(f)


if seed == 1:
    # minimal graph: 1 vertex, the six formulae from sample 1
    print("1 0")
    print(6)
    print("1=0")
    print("X|~X")
    print("A=>B=>C = (A&B)=>C")
    print("~~X => X")
    print("X => ~~X")
    print("(X => Y) = (Y | ~X)")
elif seed == 2:
    # maximal: chain on 99 vertices (D = 100), full transitive closure
    # (4851 edges), 20 formulae burning most of the 10^6 budget
    order = list(range(99))
    rng.shuffle(order)
    edges = [
        (order[i], order[j]) for i in range(99) for j in range(i + 1, 99)
    ]
    rng.shuffle(edges)
    assert ideal_count(99, edges) == 100
    emit(99, edges, make_formulas(rng, 100, 20))
elif seed == 3:
    # maximal work, different structure: two parallel chains 9 + 9
    # (D = 100), only cover edges, formulae again near the budget
    order = list(range(18))
    rng.shuffle(order)
    edges = chain_edges(order[:9]) + chain_edges(order[9:])
    assert ideal_count(18, edges) == 100
    emit(18, edges, make_formulas(rng, 100, 20))
elif seed == 4:
    # sample 2 graph and formulae
    print("6 6")
    for e in ((1, 2), (2, 3), (2, 4), (3, 5), (4, 5), (5, 6)):
        print(*e)
    print(11)
    for f in (
        "1=0",
        "X|~X",
        "A=>B=>C = (A&B)=>C",
        "~~X => X",
        "X => ~~X",
        "(X => Y) = (Y | ~X)",
        "A&(B|C) = A&B|A&C",
        "(X=>A)&(Y=>A) => X|Y=>A",
        "X = ~~X",
        "~X=~~~X",
        "~X = (X => 0)",
    ):
        print(f)
elif seed == 5:
    # small D: chain of 4 (D = 5) lets formulae use many variables
    order = [2, 0, 3, 1]
    edges = chain_edges(order, extra_closure=3, rng=rng)
    d = ideal_count(4, edges)
    emit(4, edges, make_formulas(rng, d, 20))
else:
    # random DAGs, rejected until D <= 100
    while True:
        n = rng.randint(2, 40)
        perm = list(range(n))
        rng.shuffle(perm)
        density = rng.choice([0.2, 0.5, 0.9])
        edges = [
            (perm[i], perm[j])
            for i in range(n)
            for j in range(i + 1, n)
            if rng.random() < density
        ]
        if len(edges) > 5000:
            continue
        d = ideal_count(n, edges)
        if d <= 100:
            break
    rng.shuffle(edges)
    emit(n, edges, make_formulas(rng, d, rng.randint(1, 20)))
