"""Tests for p215 (PL/Cool): interpret define/print programs.

Limits: <= 30000 defines, <= 2000 prints, lines <= 200 chars, every value
that appears during evaluation fits in |x| <= 10^9. The generator keeps a
full simulation of the interpreter, so each emitted print expression is
re-evaluated against the current define state and regenerated whenever it
would divide by zero, raise to a negative power, hit 0^0 or overflow 1e9.

Seed 1 is the minimal program, seed 2 the maximal one built around deep
substitution chains (30000 defines, 2000 prints), seed 3 a maximal one
with near-200-char expressions and heavy constant redefinition. Later
seeds are mid-size mixes with cycles, self-defines, redefinitions, case
mixing and whitespace-mangled power chains.
"""

import random
import sys

LIMIT = 10**9


class Invalid(Exception):
    pass


def normalize(tok):
    if not tok or tok[0].isdigit():
        return tok
    return tok.lower()


class State:
    """Mirror of the interpreter's substitution map."""

    def __init__(self):
        self.parent = {}

    def find(self, tok):
        chain = []
        while tok in self.parent:
            chain.append(tok)
            tok = self.parent[tok]
        for c in chain:
            self.parent[c] = tok
        return tok

    def define(self, op1, op2):
        a, b = normalize(op1), normalize(op2)
        if a not in self.parent and self.find(b) != a:
            self.parent[a] = b

    def resolve(self, tok):
        r = self.find(normalize(tok))
        if r and r[0].isdigit():
            return int(r)
        return 0


def chk(v):
    if abs(v) > LIMIT:
        raise Invalid()
    return v


def c_div(a, b):
    if b == 0:
        raise Invalid()
    sign = 1 if (a >= 0) == (b >= 0) else -1
    return sign * (abs(a) // abs(b))


def c_mod(a, b):
    if b == 0:
        raise Invalid()
    sign = 1 if (a >= 0) == (b >= 0) else -1
    rem = abs(a) % abs(b)
    return 0 if rem == 0 else sign * rem


def c_pow(base, exp):
    if exp < 0 or (base == 0 and exp == 0):
        raise Invalid()
    neg = base < 0 and exp % 2 == 1
    b = abs(base)
    if b <= 1:
        res = 1 if exp == 0 else b
    else:
        if exp > 30:
            raise Invalid()
        res = 1
        for _ in range(exp):
            res *= b
            if res > LIMIT:
                raise Invalid()
    return -res if neg else res


class Checker:
    """Validating parser: same grammar as the judge, raises Invalid on any
    division by zero, negative exponent, 0^0 or value outside [-1e9, 1e9]."""

    def __init__(self, s, state):
        self.s = s
        self.pos = 0
        self.state = state

    def skip(self):
        while self.pos < len(self.s) and self.s[self.pos] == ' ':
            self.pos += 1

    def read_token(self):
        self.skip()
        start = self.pos
        if self.pos < len(self.s) and self.s[self.pos].isdigit():
            while self.pos < len(self.s) and self.s[self.pos].isdigit():
                self.pos += 1
        elif self.pos < len(self.s) and self.s[self.pos].isalpha():
            while self.pos < len(self.s) and self.s[self.pos].isalnum():
                self.pos += 1
        return self.s[start:self.pos]

    def expr(self):
        val = self.term()
        while True:
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] in "+-":
                op = self.s[self.pos]
                self.pos += 1
                r = self.term()
                val = chk(val + r if op == '+' else val - r)
            else:
                return val

    def term(self):
        val = self.power()
        while True:
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] in "*/%":
                op = self.s[self.pos]
                self.pos += 1
                r = self.power()
                if op == '*':
                    val = chk(val * r)
                elif op == '/':
                    val = c_div(val, r)
                else:
                    val = c_mod(val, r)
            else:
                return val

    def power(self):
        val = self.unary()
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '^':
            self.pos += 1
            return c_pow(val, self.power())
        return val

    def unary(self):
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '-':
            self.pos += 1
            return -self.unary()
        if self.pos < len(self.s) and self.s[self.pos] == '+':
            self.pos += 1
            return self.unary()
        return self.atom()

    def atom(self):
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '(':
            self.pos += 1
            val = self.expr()
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] == ')':
                self.pos += 1
            return val
        return self.state.resolve(self.read_token())


def random_case(rng, name):
    return "".join(
        c.upper() if rng.random() < 0.4 else c for c in name
    )


def gen_tokens(rng, depth, names):
    """Random expression as a token list."""
    kind = rng.random()
    if depth <= 0 or kind < 0.35:
        r = rng.random()
        if r < 0.45:
            return [str(rng.randint(0, 20))]
        if r < 0.6:
            return [str(rng.randint(0, 10**9))]
        if names and r < 0.9:
            return [random_case(rng, rng.choice(names))]
        return [random_case(rng, "u" + str(rng.randint(0, 99)))]
    if kind < 0.45:
        return ["-"] * rng.randint(1, 3) + gen_tokens(rng, depth - 1, names)
    if kind < 0.55:
        return ["("] + gen_tokens(rng, depth - 1, names) + [")"]
    if kind < 0.68:
        base = ["("] + gen_tokens(rng, depth - 1, names) + [")"]
        return base + ["^", str(rng.randint(0, 3))]
    op = rng.choice(["+", "-", "*", "/", "%", "+", "-", "*"])
    return (gen_tokens(rng, depth - 1, names) + [op]
            + gen_tokens(rng, depth - 1, names))


def tokens_to_str(rng, tokens):
    parts = []
    for t in tokens:
        if parts and rng.random() < 0.25:
            parts.append(" " * rng.randint(1, 2))
        parts.append(t)
    return "".join(parts)


def make_print(rng, state, names, max_expr_len=180, min_len=0):
    for _ in range(500):
        depth = rng.randint(1, 6)
        s = tokens_to_str(rng, gen_tokens(rng, depth, names))
        if len(s) > max_expr_len or len(s) < min_len:
            continue
        try:
            Checker(s, state).expr()
        except Invalid:
            continue
        return "print " + s
    return "print 1+1"


def emit_define(rng, state, op1, op2, out):
    out.append(f"define {op1} {op2}")
    state.define(op1, op2)


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)
    out = []
    state = State()

    if seed == 1:
        print("print 0")
        return

    if seed == 2:
        # Deep substitution chains: 30000 defines, mostly v_i -> v_{i+1},
        # some chains ending in constants, plus cycle/redefine attempts.
        n_def = 30000
        chain_len = 27000
        heads = []
        i = 0
        while i < chain_len:
            length = rng.randint(500, 4000)
            length = min(length, chain_len - i)
            for j in range(i, i + length - 1):
                emit_define(rng, state, f"v{j:05d}", f"v{j + 1:05d}", out)
            tail = f"v{i + length - 1:05d}"
            if rng.random() < 0.5:
                emit_define(rng, state, tail, str(rng.randint(0, 10**6)), out)
            else:
                emit_define(rng, state, tail, "loose" + str(i), out)
            heads.append(f"v{i:05d}")
            i += length
        while len(out) < n_def:
            r = rng.random()
            if r < 0.3:
                # redefine attempt (ignored)
                j = rng.randint(0, chain_len - 2)
                emit_define(rng, state, f"v{j:05d}",
                            str(rng.randint(0, 100)), out)
            elif r < 0.5:
                # cycle attempt: tail -> head of same chain
                h = rng.choice(heads)
                emit_define(rng, state, "loose" + h[1:].lstrip("0").zfill(1),
                            h, out)
            elif r < 0.8:
                emit_define(rng, state, str(rng.randint(0, 500)),
                            str(rng.randint(0, 10**4)), out)
            else:
                emit_define(rng, state, "w" + str(rng.randint(0, 9999)),
                            rng.choice(heads), out)
        names = heads + [f"v{rng.randint(0, chain_len - 1):05d}"
                         for _ in range(200)]
        for _ in range(2000):
            out.append(make_print(rng, state, names, 160))
        sys.stdout.write("\n".join(out) + "\n")
        return

    if seed == 3:
        # Heavy constant redefinition plus 2000 near-maximal-length prints.
        names = []
        n_def = 30000
        prints_left = 2000
        total = n_def + prints_left
        defined = 0
        for k in range(total):
            if (defined < n_def and rng.random() < n_def / total) or \
                    prints_left == 0:
                r = rng.random()
                if r < 0.45:
                    emit_define(rng, state, str(rng.randint(0, 3000)),
                                str(rng.randint(0, 3000)), out)
                elif r < 0.7:
                    v = "c" + str(rng.randint(0, 4999))
                    emit_define(rng, state, v, str(rng.randint(0, 3000)),
                                out)
                    names.append(v)
                elif r < 0.9:
                    v = "c" + str(rng.randint(0, 4999))
                    w = "c" + str(rng.randint(0, 4999))
                    emit_define(rng, state, v, w, out)
                    names.append(v)
                else:
                    v = "c" + str(rng.randint(0, 4999))
                    emit_define(rng, state, str(rng.randint(0, 3000)), v,
                                out)
                defined += 1
            else:
                out.append(make_print(rng, state, names[-300:], 190, 120))
                prints_left -= 1
        sys.stdout.write("\n".join(out) + "\n")
        return

    # Mid-size adversarial mixes.
    n_lines = rng.randint(150, 900)
    names = []
    for _ in range(n_lines):
        r = rng.random()
        if r < 0.45:
            t = rng.random()
            if t < 0.25:
                emit_define(rng, state, str(rng.randint(0, 50)),
                            str(rng.randint(0, 10**9)), out)
            elif t < 0.5:
                v = random_case(rng, "x" + str(rng.randint(0, 60)))
                emit_define(rng, state, v, str(rng.randint(0, 1000)), out)
                names.append(v.lower())
            elif t < 0.7:
                v = "y" + str(rng.randint(0, 60))
                w = rng.choice(names) if names and rng.random() < 0.7 \
                    else "z" + str(rng.randint(0, 60))
                emit_define(rng, state, random_case(rng, v),
                            random_case(rng, w), out)
                names.append(v)
            elif t < 0.8 and names:
                # cycle / self-define attempts
                v = rng.choice(names)
                w = v if rng.random() < 0.4 else rng.choice(names)
                emit_define(rng, state, random_case(rng, v),
                            random_case(rng, w), out)
            else:
                emit_define(rng, state, str(rng.randint(0, 50)),
                            random_case(rng, "x" + str(rng.randint(0, 60))),
                            out)
        elif r < 0.55:
            # whitespace-mangled right-assoc power chain
            base = rng.randint(0, 3)
            line = f"print {base}" + "".join(
                f"^ {rng.randint(0, 2)}" for _ in range(rng.randint(1, 3)))
            try:
                Checker(line[6:], state).expr()
                out.append(line)
            except Invalid:
                out.append(make_print(rng, state, names))
        else:
            out.append(make_print(rng, state, names))
    sys.stdout.write("\n".join(out) + "\n")


main()
