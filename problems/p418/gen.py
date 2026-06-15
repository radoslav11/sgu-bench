"""Tests for p418 (Deducing Grammar): emit a random restricted Pascal parser.

Input <= 10000 bytes, words <= 20 chars, output guaranteed <= 10000 bytes.
Seed 1 is the minimal parser; seeds 2-3 are ~10 KB programs (many routines /
deep conditional chains); later seeds vary whitespace style, letter case,
duplicate Peek characters and empty branch bodies.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

KEYWORDS = {
    "program", "parser", "procedure", "function", "forward", "var",
    "string", "integer", "char", "begin", "end", "if", "then", "else",
    "skip", "peek", "error", "inc", "pos", "length", "writeln", "halt",
    "readln", "st",
}

CHARS = [chr(c) for c in range(33, 127) if c != 39]


def random_name(used):
    while True:
        length = rng.choice([1, 2, 3, 4, 6, 10, 20])
        name = "".join(rng.choice("abcdefghijklmnopqrstuvwxyz")
                       for _ in range(length))
        if name not in KEYWORDS and name not in used:
            used.add(name)
            return name


# Segments: ("call", name) | ("if", [(char, [segments]), ...], else_error)
def gen_segments(names, depth, max_segs):
    count = rng.randint(1, max_segs)
    return [gen_segment(names, depth) for _ in range(count)]


def gen_segment(names, depth):
    if depth <= 0 or rng.random() < 0.45:
        return ("call", rng.choice(names))
    n_br = rng.randint(1, 3)
    branches = []
    dup_char = rng.choice(CHARS)
    for _ in range(n_br):
        ch = dup_char if rng.random() < 0.25 else rng.choice(CHARS)
        inner = [] if rng.random() < 0.4 else gen_segments(names, depth - 1, 2)
        branches.append((ch, inner))
    return ("if", branches, rng.random() < 0.5)


def expand_segments(segments):
    result = [[]]
    for seg in segments:
        alts = expand_segment(seg)
        result = [p + s for p in result for s in alts]
        if len(result) > 200:
            return None
    return result


def expand_segment(seg):
    if seg[0] == "call":
        return [[("n", seg[1])]]
    _, branches, else_error = seg
    out = []
    for ch, inner in branches:
        sub = expand_segments(inner)
        if sub is None:
            return [[("n", "x")]] * 999
        out.extend([[("t", ch)] + p for p in sub])
    if not else_error:
        out.append([])
    return out


def render_production(prod):
    parts, i = [], 0
    while i < len(prod):
        if prod[i][0] == "t":
            chars = []
            while i < len(prod) and prod[i][0] == "t":
                chars.append(prod[i][1])
                i += 1
            parts.append("'" + "".join(chars) + "'")
        else:
            parts.append("<" + prod[i][1] + ">")
            i += 1
    return "".join(parts)


def output_size(name, body):
    prods = expand_segments(body)
    if prods is None:
        return 10 ** 9
    alts = sorted(render_production(p) for p in prods)
    return len("<" + name + ">::=" + "|".join(alts)) + 1


# Token-level Pascal rendering with random case and whitespace.
def W(w):  # word token with randomized case
    style = rng.random()
    if style < 0.5:
        return ("w", w.capitalize())
    if style < 0.8:
        return ("w", w.lower())
    if style < 0.9:
        return ("w", w.upper())
    return ("w", "".join(c.upper() if rng.random() < 0.5 else c for c in w))


def P(c):
    return ("p", c)


def L(s):
    return ("p", "'" + s + "'")


def seg_tokens(seg):
    toks = []
    if seg[0] == "call":
        toks += [W(seg[1]), P(";")]
        return toks
    _, branches, else_error = seg
    first = True
    for ch, inner in branches:
        if not first:
            toks += [W("else")]
        toks += [W("if"), W("peek"), P("="), L(ch), W("then"), W("begin"),
                 W("skip"), P(";")]
        for s in inner:
            toks += seg_tokens(s)
        toks += [W("end")]
        first = False
    if else_error:
        toks += [W("else"), W("error"), P(";")]
    else:
        toks += [P(";")]
    return toks


def routine_tokens(name, body):
    toks = [W("procedure"), W(name), P(";"), W("begin")]
    for s in body:
        toks += seg_tokens(s)
    toks += [W("end"), P(";")]
    return toks


def program_tokens(routines):
    toks = [W("program"), W("parser"), P(";")]
    toks += [W("procedure"), W("skip"), P(";"), W("forward"), P(";")]
    toks += [W("function"), W("peek"), P(":"), W("char"), P(";"),
             W("forward"), P(";")]
    toks += [W("procedure"), W("error"), P(";"), W("forward"), P(";")]
    for name, _ in routines:
        toks += [W("procedure"), W(name), P(";"), W("forward"), P(";")]
    for name, body in routines:
        toks += routine_tokens(name, body)
    toks += [W("var"), W("st"), P(":"), W("string"), P(";"),
             W("pos"), P(":"), W("integer"), P(";")]
    toks += [W("procedure"), W("error"), P(";"), W("begin"),
             W("writeln"), P("("), L("NO"), P(")"), P(";"),
             W("halt"), P(";"), W("end"), P(";")]
    toks += [W("procedure"), W("skip"), P(";"), W("begin"),
             W("inc"), P("("), W("pos"), P(")"), P(";"),
             W("if"), W("pos"), P(">"), W("length"), P("("), W("st"), P(")"),
             W("then"), W("error"), P(";"), W("end"), P(";")]
    toks += [W("function"), W("peek"), P(":"), W("char"), P(";"), W("begin"),
             W("peek"), P(":"), P("="), W("st"), P("["), W("pos"), P("]"),
             P(";"), W("end"), P(";")]
    toks += [W("begin"), W("readln"), P("("), W("st"), P(")"), P(";"),
             W("st"), P(":"), P("="), W("st"), P("+"), L("#"), P(";"),
             W("pos"), P(":"), P("="), P("1"), P(";"),
             W("parse"), P(";"),
             W("if"), W("pos"), P("="), W("length"), P("("), W("st"), P(")"),
             W("then"), W("writeln"), P("("), L("YES"), P(")"),
             W("else"), W("writeln"), P("("), L("NO"), P(")"), P(";"),
             W("end"), P(".")]
    return toks


def render_tokens(toks, style):
    # style: 0 = compact one line, 1 = airy multi-line, 2 = mixed
    out = []
    prev_kind = None
    line_len = 0
    for kind, text in toks:
        sep = ""
        if prev_kind is not None:
            need = prev_kind == "w" and kind == "w"
            if style == 0:
                sep = " " if need else ""
            elif style == 1:
                sep = "\n" if rng.random() < 0.3 else " "
                if not need and rng.random() < 0.5:
                    sep = ""
            else:
                if need:
                    sep = " " if rng.random() < 0.8 else "\n"
                elif rng.random() < 0.2:
                    sep = rng.choice([" ", "  ", "\n"])
            if line_len > 200 and "\n" not in sep:
                if need:
                    sep = "\n"
                elif sep == "":
                    sep = "\n"
        out.append(sep + text)
        line_len = len(text) if "\n" in sep else line_len + len(sep + text)
        prev_kind = kind
    return "".join(out) + "\n"


def build_program(n_routines, depth, max_segs, in_budget, out_budget, style):
    used = set()
    names = [
        "parse" if i == 0 else random_name(used) for i in range(n_routines)
    ]
    routines = []
    out_total = 0
    for name in names:
        for _ in range(50):
            body = gen_segments(names, depth, max_segs)
            sz = output_size(name, body)
            if sz <= 400 and out_total + sz <= out_budget:
                break
        else:
            body = [("call", rng.choice(names))]
            sz = output_size(name, body)
        out_total += sz
        routines.append((name, body))
    text = render_tokens(program_tokens(routines), style)
    while len(text) > in_budget and len(routines) > 1:
        routines.pop()
        text = render_tokens(program_tokens(routines), style)
    return text


if seed == 1:
    # Minimal parser: a single routine Parse that calls itself.
    routines = [("parse", [("call", "parse")])]
    sys.stdout.write(render_tokens(program_tokens(routines), 0))
elif seed == 2:
    # Maximal: many routines, compact rendering, ~10 KB input.
    sys.stdout.write(build_program(70, 2, 2, 10000, 9000, 0))
elif seed == 3:
    # Maximal with different structure: fewer, deeper routines, airy layout.
    sys.stdout.write(build_program(55, 4, 3, 10000, 9000, 1))
else:
    n = rng.randint(1, 20)
    depth = rng.randint(1, 4)
    style = rng.choice([0, 1, 2])
    sys.stdout.write(build_program(n, depth, rng.randint(1, 3), 10000,
                                   8000, style))
