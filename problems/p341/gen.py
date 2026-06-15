"""Tests for p341 (Circuits): <= 5000 junctions, <= 500 value lines, file < 320 KB.

The combinational part must be acyclic, so non-DFF gates only read from
already-created gates, primary inputs and DFF outputs; DFF inputs may
reference any junction (feedback). Seed 1 is minimal, seed 2 maxes out
junction count and tick count, seed 3 is a 4900-deep NOT/AND chain with
DFF feedback evaluated for 500 ticks (worst recursion / eval cost).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

GATES = ["NOT", "DFF", "AND", "NAND", "OR", "NOR"]
NAME_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"


def make_names(count, long_frac=0.02):
    names = set()
    result = []
    while len(result) < count:
        if rng.random() < long_frac:
            ln = rng.randint(20, 63)
        else:
            ln = rng.randint(2, 6)
        name = rng.choice(NAME_CHARS[:52]) + "".join(
            rng.choice(NAME_CHARS) for _ in range(ln - 1)
        )
        if name not in names and name != "INPUT" and name != "OUTPUT":
            names.add(name)
            result.append(name)
    return result


def build_circuit(n_inputs, n_dff, n_comb, n_outputs, n_ticks, max_args,
                  chain=False, long_frac=0.02):
    total = n_inputs + n_dff + n_comb
    names = make_names(total, long_frac)
    inputs = names[:n_inputs]
    dffs = names[n_inputs:n_inputs + n_dff]
    combs = names[n_inputs + n_dff:]

    lines = []
    pool = inputs + dffs  # junctions usable by combinational gates so far
    for idx, name in enumerate(combs):
        if chain:
            src = combs[idx - 1] if idx > 0 else rng.choice(inputs + dffs)
            op = rng.choice(["NOT", "AND", "OR", "NAND", "NOR"])
            args = [src]
        else:
            op = rng.choice(["NOT", "AND", "NAND", "OR", "NOR"])
            if op == "NOT":
                args = [rng.choice(pool)]
            else:
                k = rng.randint(1, max_args)
                args = [rng.choice(pool) for _ in range(k)]
        sep = ", " if rng.random() < 0.8 else ","
        lines.append(f"{name} = {op}({sep.join(args)})")
        pool.append(name)

    for name in dffs:
        if chain:
            src = combs[-1] if combs else rng.choice(inputs)
        else:
            src = rng.choice(pool)
        lines.append(f"{name} = DFF({src})")

    rng.shuffle(lines)

    decl = [f"INPUT({name})" for name in inputs]
    outs = rng.sample(pool, min(n_outputs, len(pool)))
    decl += [f"OUTPUT({name})" for name in outs]
    # Interleave declarations into the definition list at random spots.
    for d in decl:
        lines.insert(rng.randint(0, len(lines)), d)

    # Sprinkle comments and blank lines.
    for _ in range(rng.randint(2, 8)):
        pos = rng.randint(0, len(lines))
        lines.insert(pos, rng.choice(["", "# random comment 123", "#", "#_x"]))

    print("\n".join(lines))
    print("INPUT VALUES")
    for _ in range(n_ticks):
        print("".join(rng.choice("01") for _ in range(n_inputs)))


if seed == 1:
    print("INPUT(a)")
    print("OUTPUT(a)")
    print("INPUT VALUES")
    print("0")
elif seed == 2:
    build_circuit(n_inputs=150, n_dff=850, n_comb=4000, n_outputs=120,
                  n_ticks=500, max_args=3, long_frac=0.01)
elif seed == 3:
    build_circuit(n_inputs=2, n_dff=90, n_comb=4900, n_outputs=200,
                  n_ticks=500, max_args=1, chain=True, long_frac=0.0)
elif seed == 4:
    # Few gates with very many (duplicate) arguments.
    build_circuit(n_inputs=30, n_dff=20, n_comb=300, n_outputs=30,
                  n_ticks=500, max_args=60, long_frac=0.1)
else:
    build_circuit(n_inputs=rng.randint(1, 40),
                  n_dff=rng.randint(0, 60),
                  n_comb=rng.randint(1, 400),
                  n_outputs=rng.randint(1, 40),
                  n_ticks=rng.randint(1, 100),
                  max_args=rng.randint(1, 8),
                  long_frac=0.1)
