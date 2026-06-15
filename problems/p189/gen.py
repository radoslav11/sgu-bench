"""Tests for p189 (Perl-like Substr): simulate a tiny Perl interpreter.

N <= 20 init lines (values up to 255 chars of alnum + " ,.-_:!?"),
M <= 300 program lines (each <= 255 chars), at most 100 variables,
values never exceed 1000 chars during execution. The generator keeps a
live variable table so every substr() call it emits is "correct"
(non-empty, inside the string), mirroring the guarantee.

Seed 1 is the one-variable minimum, seed 2 a maximum stressing string
growth to 1000 chars with heavy spacing (including "- 3" split minus
signs), seed 3 a maximum with many variables, uninitialized reads and
all six statement types. Seed 4 is the statement sample. Later seeds
are random mixes with different spacing habits.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

VALUE_CHARS = string.ascii_letters + string.digits + " ,.-_:!?"
NAME_CHARS = string.ascii_letters + string.digits


def make_name(rng, used):
    while True:
        name = "$" + rng.choice(string.ascii_letters) + "".join(
            rng.choice(NAME_CHARS) for _ in range(rng.randint(0, 17))
        )
        if name not in used:
            used.add(name)
            return name


def make_value(rng, max_len):
    return "".join(
        rng.choice(VALUE_CHARS) for _ in range(rng.randint(1, max_len))
    )


def sp(rng, fancy):
    # space run allowed between any two tokens of a program line
    if not fancy:
        return ""
    return " " * rng.choice((0, 0, 0, 1, 1, 2))


def num_tok(rng, value, fancy):
    if fancy and value < 0 and rng.random() < 0.3:
        return "-" + sp(rng, True) + str(-value)
    return str(value)


def substr_call(rng, name, length, fancy, force_full=False):
    # choose 0 <= start < end <= length, render with random arg styles;
    # returns (text, start, end)
    if force_full:
        start, end = 0, length
    else:
        start = rng.randrange(length)
        end = rng.randint(start + 1, length)
    if rng.random() < 0.5 and start > 0:
        off = start - length
    else:
        off = start
    args = name + sp(rng, fancy) + "," + sp(rng, fancy)
    args += num_tok(rng, off, fancy)
    if end == length and rng.random() < 0.5:
        pass  # omit count
    else:
        cnt = end - start if (end == length or rng.random() < 0.6) else end - length
        args += sp(rng, fancy) + "," + sp(rng, fancy) + num_tok(rng, cnt, fancy)
    text = "substr(" + sp(rng, fancy) + args + sp(rng, fancy) + ")"
    return text, start, end


def gen_program(rng, n_init, n_vars, m_lines, val_len, fancy, grow):
    used = set()
    names = [make_name(rng, used) for _ in range(n_vars)]
    vars_ = {name: "" for name in names}
    init_names = names[:n_init]
    init_lines = []
    for name in init_names:
        value = make_value(rng, val_len)
        vars_[name] = value
        line = (name + sp(rng, fancy) + " = " + sp(rng, fancy) + '"'
                + value + '"' + sp(rng, fancy) + ";")
        init_lines.append(line)

    prog_lines = []
    n_prints = 0
    while len(prog_lines) < m_lines:
        last = len(prog_lines) == m_lines - 1
        nonempty = [v for v in names if vars_[v]]
        if not nonempty:
            break
        op = rng.randint(1, 6)
        if last and n_prints == 0:
            op = 1
        line = None
        if op == 1:
            v = rng.choice(nonempty if (last or rng.random() < 0.9)
                           else names)
            line = ("print(" + sp(rng, fancy) + v + sp(rng, fancy) + ")"
                    + sp(rng, fancy) + ";")
            n_prints += 1
        elif op == 2:
            v = rng.choice(nonempty)
            call, s, e = substr_call(rng, v, len(vars_[v]), fancy,
                                     force_full=last and rng.random() < 0.5)
            line = ("print(" + sp(rng, fancy) + call + sp(rng, fancy) + ")"
                    + sp(rng, fancy) + ";")
            n_prints += 1
        elif op == 3:
            v1 = rng.choice(names)
            v2 = rng.choice(names if rng.random() < 0.2 else nonempty)
            if not vars_[v2] and vars_[v1] and len(nonempty) <= 1:
                continue
            line = (v1 + sp(rng, fancy) + "=" + sp(rng, fancy) + v2
                    + sp(rng, fancy) + ";")
            if len(line) <= 255:
                vars_[v1] = vars_[v2]
        elif op == 4:
            v1 = rng.choice(names)
            v2 = rng.choice(nonempty)
            call, s, e = substr_call(rng, v2, len(vars_[v2]), fancy)
            line = (v1 + sp(rng, fancy) + "=" + sp(rng, fancy) + call
                    + sp(rng, fancy) + ";")
            if len(line) <= 255:
                vars_[v1] = vars_[v2][s:e]
        else:
            v1 = rng.choice(nonempty)
            call1, s1, e1 = substr_call(rng, v1, len(vars_[v1]), fancy)
            if op == 5:
                v2 = rng.choice(names if rng.random() < 0.1 else nonempty)
                new = vars_[v2]
                rhs = v2
            else:
                v2 = rng.choice(nonempty)
                if grow and rng.random() < 0.7:
                    s2, e2 = 0, len(vars_[v2])
                    call2, _, _ = substr_call(rng, v2, len(vars_[v2]),
                                              fancy, force_full=True)
                else:
                    call2, s2, e2 = substr_call(rng, v2, len(vars_[v2]),
                                                fancy)
                new = vars_[v2][s2:e2]
                rhs = call2
            result = vars_[v1][:s1] + new + vars_[v1][e1:]
            if len(result) > 1000:
                continue
            if not result and len(nonempty) <= 1:
                continue
            line = (call1 + sp(rng, fancy) + "=" + sp(rng, fancy) + rhs
                    + sp(rng, fancy) + ";")
            if len(line) <= 255:
                vars_[v1] = result
        if line is not None and len(line) <= 255:
            prog_lines.append(line)

    print(len(init_lines), len(prog_lines))
    for line in init_lines:
        print(line)
    for line in prog_lines:
        print(line)


if seed == 1:
    print(1, 1)
    print('$a = "x";')
    print('print($a);')
elif seed == 2:
    gen_program(rng, n_init=20, n_vars=24, m_lines=300, val_len=255,
                fancy=True, grow=True)
elif seed == 3:
    gen_program(rng, n_init=20, n_vars=100, m_lines=300, val_len=255,
                fancy=True, grow=False)
elif seed == 4:
    print(2, 9)
    print('$a = "0123456789";')
    print('$b = "abcdefghigklmn";')
    print('print($a);')
    print('print( substr($b, 1, 2));')
    print('substr($b, 0, 1) = substr($a, 2, 7);')
    print('$c = $b;')
    print('print(substr($c,0));')
    print('print(substr($a,  - 2,  - 1));')
    print('print(substr($a, -6, 2));')
    print('print(substr($a,  - 5));')
    print('print(substr($a, 1, 2));')
elif seed == 5:
    # plain spacing, mid-size
    gen_program(rng, n_init=rng.randint(1, 20), n_vars=30,
                m_lines=rng.randint(50, 300), val_len=80,
                fancy=False, grow=True)
else:
    gen_program(rng, n_init=rng.randint(1, 20),
                n_vars=rng.randint(2, 100),
                m_lines=rng.randint(1, 300),
                val_len=rng.choice((10, 80, 255)),
                fancy=rng.random() < 0.7,
                grow=rng.random() < 0.5)
