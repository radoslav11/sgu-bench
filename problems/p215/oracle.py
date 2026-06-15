import sys

# parent[a] = b means "replace a with b"
parent = {}


def normalize(tok: str) -> str:
    """Lowercase identifiers (case-insensitive). Keep numbers unchanged."""
    if not tok or tok[0].isdigit():
        return tok
    return tok.lower()


def find(tok: str) -> str:
    """Follow substitution links with path compression (iterative)."""
    chain = []
    while tok in parent:
        chain.append(tok)
        tok = parent[tok]
    for c in chain:
        parent[c] = tok
    return tok


def resolve_token(tok: str) -> int:
    """
    Resolve a token used in an expression:
    - apply normalize + recursive substitution (find)
    - if ends at a numeric constant => return it
    - else => treat as 0
    """
    r = find(normalize(tok))
    if r and r[0].isdigit():
        return int(r)
    return 0


def power(base: int, exp: int) -> int:
    """Fast exponentiation; exp is non-negative. Handle negative base sign."""
    neg = (base < 0 and (exp % 2 == 1))
    b = abs(base)
    res = 1
    e = exp
    while e > 0:
        if e & 1:
            res *= b
        b *= b
        e >>= 1
    return -res if neg else res


def custom_div(a: int, b: int) -> int:
    """Division per statement: sign(a/b) * (|a|//|b|)."""
    sign = 1 if (a >= 0) == (b >= 0) else -1
    return sign * (abs(a) // abs(b))


def custom_mod(a: int, b: int) -> int:
    """Modulo per statement: sign(a/b) * (|a|%|b|), but 0 remainder stays 0."""
    sign = 1 if (a >= 0) == (b >= 0) else -1
    rem = abs(a) % abs(b)
    return 0 if rem == 0 else sign * rem


class Parser:
    """Recursive descent parser for PL/Cool expressions."""

    def __init__(self, s: str):
        self.s = s
        self.pos = 0

    def skip(self):
        """Skip spaces."""
        n = len(self.s)
        while self.pos < n and self.s[self.pos] == ' ':
            self.pos += 1

    def read_token(self) -> str:
        """Read a number or identifier token."""
        self.skip()
        if self.pos >= len(self.s):
            return ""
        start = self.pos
        c = self.s[self.pos]
        if c.isdigit():
            while self.pos < len(self.s) and self.s[self.pos].isdigit():
                self.pos += 1
        elif c.isalpha():
            while self.pos < len(self.s) and self.s[self.pos].isalnum():
                self.pos += 1
        return self.s[start:self.pos]

    def parse_expr(self) -> int:
        """expr := term (('+'|'-') term)*"""
        val = self.parse_term()
        while True:
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] in "+-":
                op = self.s[self.pos]
                self.pos += 1
                r = self.parse_term()
                val = val + r if op == '+' else val - r
            else:
                break
        return val

    def parse_term(self) -> int:
        """term := power (('*'|'/'|'%') power)*"""
        val = self.parse_power()
        while True:
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] in "*/%":
                op = self.s[self.pos]
                self.pos += 1
                r = self.parse_power()
                if op == '*':
                    val *= r
                elif op == '/':
                    val = custom_div(val, r)
                else:
                    val = custom_mod(val, r)
            else:
                break
        return val

    def parse_power(self) -> int:
        """power := unary ('^' power)?  (right-associative)"""
        val = self.parse_unary()
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '^':
            self.pos += 1
            return power(val, self.parse_power())
        return val

    def parse_unary(self) -> int:
        """unary := ('+'|'-') unary | atom"""
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '-':
            self.pos += 1
            return -self.parse_unary()
        if self.pos < len(self.s) and self.s[self.pos] == '+':
            self.pos += 1
            return self.parse_unary()
        return self.parse_atom()

    def parse_atom(self) -> int:
        """atom := '(' expr ')' | token"""
        self.skip()
        if self.pos < len(self.s) and self.s[self.pos] == '(':
            self.pos += 1
            val = self.parse_expr()
            self.skip()
            if self.pos < len(self.s) and self.s[self.pos] == ')':
                self.pos += 1
            return val
        return resolve_token(self.read_token())


def main():
    lines = sys.stdin.read().splitlines()

    out = []
    for line in lines:
        # skip leading spaces
        i = 0
        while i < len(line) and line[i] == ' ':
            i += 1

        if line.startswith("print", i):
            expr = line[i + 5:]               # after keyword
            p = Parser(expr)
            out.append(str(p.parse_expr()))
        elif line.startswith("define", i):
            rest = line[i + 6:]
            parts = rest.split()
            if len(parts) >= 2:
                op1, op2 = parts[0], parts[1]
                a = normalize(op1)
                b = normalize(op2)

                # Ignore redefinitions
                if a not in parent:
                    # Cycle check: if b ultimately becomes a, ignore
                    rb = find(b)
                    if rb != a:
                        parent[a] = b

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
