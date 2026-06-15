"""Checker for p202: usage checker.py <in> <out> <ans>; exit 0 = accept.

The move count L is deterministic (Frame-Stewart optimum) and must match
the reference. The move list is a certificate: it is simulated from the
initial position (all N discs on peg 1); every move must take the top
disc of its source peg, place it on a strictly larger disc or an empty
peg, and use the correct "atop" clause. After exactly L moves all discs
must sit on peg M. Any valid optimal scenario is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    expected_l = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        l = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if l != expected_l:
        reject(f"optimal move count is {expected_l}, got {l}")

    pegs = [[] for _ in range(m + 1)]
    pegs[1] = list(range(n, 0, -1))

    pos = 1
    tokens = out_tokens

    def take(expect_word=None):
        nonlocal pos
        if pos >= len(tokens):
            reject("output ended in the middle of a move")
        tok = tokens[pos]
        pos += 1
        if expect_word is not None and tok != expect_word:
            reject(f"expected '{expect_word}', got '{tok}'")
        return tok

    def take_int():
        tok = take()
        try:
            return int(tok)
        except ValueError:
            reject(f"expected an integer, got '{tok}'")

    for move_no in range(1, l + 1):
        take("move")
        disc = take_int()
        take("from")
        src = take_int()
        take("to")
        dst = take_int()
        if not (1 <= src <= m) or not (1 <= dst <= m):
            reject(f"move {move_no}: peg out of range")
        if src == dst:
            reject(f"move {move_no}: source equals target")
        if not pegs[src]:
            reject(f"move {move_no}: peg {src} is empty")
        if pegs[src][-1] != disc:
            reject(
                f"move {move_no}: top of peg {src} is {pegs[src][-1]},"
                f" not {disc}"
            )
        if pegs[dst]:
            top = pegs[dst][-1]
            if top < disc:
                reject(
                    f"move {move_no}: cannot place disc {disc} atop"
                    f" smaller disc {top}"
                )
            take("atop")
            atop = take_int()
            if atop != top:
                reject(
                    f"move {move_no}: 'atop {atop}' but top of peg"
                    f" {dst} is {top}"
                )
        else:
            if pos < len(tokens) and tokens[pos] == "atop":
                reject(f"move {move_no}: 'atop' clause on an empty peg")
        pegs[src].pop()
        pegs[dst].append(disc)

    if pos != len(tokens):
        reject("trailing output after the last move")
    if len(pegs[m]) != n:
        reject(f"after {l} moves only {len(pegs[m])} discs are on peg {m}")

    sys.exit(0)


if __name__ == "__main__":
    main()
