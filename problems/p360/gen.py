"""Tests for p360 (B++ Interpreter).

Input: N x M map (1..50, cells ~ / * / one R) followed by a B++ program:
functions a..z with 0-2 capital-letter params (not L/R/C), bodies made of
L/R/C commands, parameter references, and calls forming a DAG (no
recursion). Whitespace may appear between any tokens. Guarantees kept by
the generator: <= 1000 robot commands executed, file <= 10000 bytes.

The generator builds a random function DAG (function i only calls
functions with larger index, entry m is index 0), simulates the expansion
to count executed commands, and rejection-samples until the count fits
the 1000-command budget. Whitespace is sprinkled between tokens.

Seed plan:
- seed 1: 1x1 map, program m(){C} (minimal).
- seed 2: 50x50 dense-star map, deep call DAG executing ~1000 commands,
  plus junk functions padding the file toward the 10000-byte cap.
- seed 3: 50x50 map, different shape: long literal bodies, heavy
  whitespace shredding, ~1000 commands.
- seed 4: 1x50 corridor (robot pinned against borders, many ignored Cs).
- seed 5: 50x1 column map.
- seeds 6+: random mid-size maps and programs.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

PARAM_LETTERS = [c for c in string.ascii_uppercase if c not in "LRC"]


def make_map(rng, n, m, star_prob):
    grid = [
        ["*" if rng.random() < star_prob else "~" for _ in range(m)]
        for _ in range(n)
    ]
    ri, rj = rng.randrange(n), rng.randrange(m)
    grid[ri][rj] = "R"
    return grid


def make_program(rng, nfuncs, max_ops, call_prob):
    # Function i may only call functions with larger index -> DAG.
    names = ["m"] + rng.sample(
        [c for c in string.ascii_lowercase if c != "m"], nfuncs - 1
    )
    funcs = []
    for i, name in enumerate(names):
        arity = 0 if i == 0 else rng.randint(0, 2)
        params = rng.sample(PARAM_LETTERS, arity)
        funcs.append({"name": name, "params": params, "body": []})

    for i, f in enumerate(funcs):
        nops = rng.randint(1, max_ops)
        for _ in range(nops):
            r = rng.random()
            if r < call_prob and i + 1 < nfuncs:
                j = rng.randint(i + 1, nfuncs - 1)
                callee = funcs[j]
                actuals = [
                    rng.choice(["L", "R", "C"] + f["params"])
                    for _ in callee["params"]
                ]
                f["body"].append(("call", j, actuals))
            elif f["params"] and r < call_prob + 0.2:
                f["body"].append(("arg", rng.choice(f["params"])))
            else:
                f["body"].append(("cmd", rng.choice("LRC")))
    return funcs


def count_commands(funcs, limit=1000):
    # Returns executed command count of m(), or None if it exceeds limit.
    total = 0

    def run(idx, env):
        nonlocal total
        for op in funcs[idx]["body"]:
            if op[0] in ("cmd", "arg"):
                total += 1
                if total > limit:
                    raise OverflowError
            else:
                _, j, actuals = op
                callee = funcs[j]
                new_env = {}
                for p, a in zip(callee["params"], actuals):
                    new_env[p] = env.get(a, a)
                run(j, new_env)

    try:
        run(0, {})
    except OverflowError:
        return None
    return total


def render(funcs, rng, shred):
    # shred: probability of inserting whitespace between adjacent tokens.
    tokens = []
    for f in funcs:
        tokens += [f["name"], "("]
        for k, p in enumerate(f["params"]):
            if k:
                tokens.append(",")
            tokens.append(p)
        tokens += [")", "{"]
        for op in f["body"]:
            if op[0] == "cmd" or op[0] == "arg":
                tokens.append(op[1])
            else:
                _, j, actuals = op
                tokens += [funcs[j]["name"], "("]
                for k, a in enumerate(actuals):
                    if k:
                        tokens.append(",")
                    tokens.append(a)
                tokens.append(")")
        tokens.append("}")

    out = []
    for t in tokens:
        out.append(t)
        if rng.random() < shred:
            out.append(rng.choice([" ", "\n", "  ", " \n "]))
    return "".join(out)


def emit(grid, program_text):
    print(len(grid), len(grid[0]))
    for row in grid:
        print("".join(row))
    print(program_text)


if seed == 1:
    print(1, 1)
    print("R")
    print("m(){C}")
elif seed in (2, 3):
    n = m = 50
    grid = make_map(rng, n, m, 0.5 if seed == 2 else 0.15)
    while True:
        if seed == 2:
            funcs = make_program(rng, nfuncs=12, max_ops=8, call_prob=0.45)
        else:
            funcs = make_program(rng, nfuncs=6, max_ops=20, call_prob=0.25)
        cnt = count_commands(funcs)
        if cnt is not None and cnt >= 400:
            break
    text = render(funcs, rng, shred=0.3 if seed == 2 else 0.7)
    # Pad toward the 10000-byte file cap with never-called junk functions
    # (they call nothing, so no recursion is possible).
    used = {f["name"] for f in funcs}
    junk_names = [c for c in string.ascii_lowercase if c not in used]
    junk = []
    budget = 9000 - len(text) - 51 * 51
    for name in junk_names:
        if budget <= 0:
            break
        body = "".join(rng.choice("LRC") for _ in range(60))
        piece = f"{name}(){{{body}}}"
        junk.append(piece)
        budget -= len(piece) + 1
    emit(grid, text + "\n" + "\n".join(junk))
elif seed == 4:
    grid = make_map(rng, 1, 50, 0.4)
    while True:
        funcs = make_program(rng, nfuncs=8, max_ops=10, call_prob=0.4)
        cnt = count_commands(funcs)
        if cnt is not None and cnt >= 200:
            break
    emit(grid, render(funcs, rng, shred=0.4))
elif seed == 5:
    grid = make_map(rng, 50, 1, 0.4)
    while True:
        funcs = make_program(rng, nfuncs=8, max_ops=10, call_prob=0.4)
        cnt = count_commands(funcs)
        if cnt is not None and cnt >= 200:
            break
    emit(grid, render(funcs, rng, shred=0.4))
else:
    n, m = rng.randint(2, 50), rng.randint(2, 50)
    grid = make_map(rng, n, m, rng.uniform(0.05, 0.6))
    while True:
        funcs = make_program(
            rng,
            nfuncs=rng.randint(1, 10),
            max_ops=rng.randint(3, 15),
            call_prob=rng.uniform(0.1, 0.5),
        )
        cnt = count_commands(funcs)
        if cnt is not None:
            break
    emit(grid, render(funcs, rng, shred=rng.uniform(0.0, 0.8)))
