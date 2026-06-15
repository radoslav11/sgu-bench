"""Checker for p182: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any bracket-free, syntactically correct boolean expression over a..j of
length <= 32768 that is identical (over all 2^10 assignments) to the
input expression is accepted. Expressions are evaluated as 1024-bit
masks, one bit per assignment of the ten variables, so the equivalence
check is a single big-integer comparison.
"""

import sys

sys.setrecursionlimit(1 << 20)

FULL = (1 << 1024) - 1
VAR_MASK = {}
for v in range(10):
    m = 0
    for a in range(1024):
        if (a >> v) & 1:
            m |= 1 << a
    VAR_MASK[chr(ord('a') + v)] = m


def reject(message):
    print(message)
    sys.exit(1)


class Parser:
    def __init__(self, s, allow_paren):
        self.s = s
        self.pos = 0
        self.allow_paren = allow_paren

    def error(self, msg):
        reject(f"parse error at position {self.pos}: {msg}")

    def atom(self):
        if self.pos >= len(self.s):
            self.error("unexpected end of expression")
        c = self.s[self.pos]
        if c == '(':
            if not self.allow_paren:
                self.error("brackets are not allowed in the output")
            self.pos += 1
            r = self.low()
            if self.pos >= len(self.s) or self.s[self.pos] != ')':
                self.error("expected ')'")
            self.pos += 1
            return r
        if 'a' <= c <= 'j':
            self.pos += 1
            return VAR_MASK[c]
        self.error(f"unexpected character '{c}'")

    def high(self):
        if self.pos < len(self.s) and self.s[self.pos] == '!':
            self.pos += 1
            return FULL ^ self.high()
        return self.atom()

    def mid(self):
        r = self.high()
        while self.pos < len(self.s) and self.s[self.pos] == '&':
            self.pos += 1
            r &= self.high()
        return r

    def low(self):
        r = self.mid()
        while self.pos < len(self.s):
            s, p = self.s, self.pos
            if s.startswith('||', p):
                self.pos += 2
                r |= self.mid()
            elif s.startswith('<=>', p):
                self.pos += 3
                r = FULL ^ (r ^ self.mid())
            elif s.startswith('=>', p):
                self.pos += 2
                r = (FULL ^ r) | self.mid()
            elif s.startswith('#', p):
                self.pos += 1
                r ^= self.mid()
            else:
                break
        return r

    def parse(self):
        r = self.low()
        if self.pos != len(self.s):
            self.error("trailing characters")
        return r


def main():
    in_expr = open(sys.argv[1]).read().split()[0]
    out_tokens = open(sys.argv[2]).read().split()

    if not out_tokens:
        reject("empty output")
    if len(out_tokens) > 1:
        reject("output must be a single expression without spaces")
    out_expr = out_tokens[0]

    if len(out_expr) > 32768:
        reject(f"output length {len(out_expr)} exceeds 32768")
    allowed = set("abcdefghij!&|<=>#")
    bad = set(out_expr) - allowed
    if bad:
        reject(f"illegal characters in output: {sorted(bad)}")

    want = Parser(in_expr, allow_paren=True).parse()
    got = Parser(out_expr, allow_paren=False).parse()

    if want != got:
        diff = want ^ got
        a = diff.bit_length() - 1
        vals = "".join(str((a >> v) & 1) for v in range(10))
        reject(f"expressions differ on assignment a..j = {vals}")

    sys.exit(0)


if __name__ == "__main__":
    main()
