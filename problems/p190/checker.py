"""Checker for p190: usage checker.py <in> <out> <ans>; exit 0 = accept.

Yes/No must match the reference. A "Yes" must come with a tiling
certificate: Nh horizontal dice given by their left square (covering
(x, y) and (x+1, y)) and Nv vertical dice given by their bottom square
(covering (x, y) and (x, y+1)); together they must cover every
non-removed square exactly once. Any valid tiling is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    n = int(next(it))
    p = int(next(it))
    removed = set()
    for _ in range(p):
        x = int(next(it))
        y = int(next(it))
        removed.add((x, y))
    free = {(x, y) for x in range(1, n + 1) for y in range(1, n + 1)
            if (x, y) not in removed}

    if not out_tokens:
        reject("empty output")
    expected = ans_tokens[0].lower()
    got = out_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be Yes/No, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "no":
        if len(out_tokens) > 1:
            reject("trailing output after 'No'")
        sys.exit(0)

    pos = 1

    def next_int(what):
        nonlocal pos
        if pos >= len(out_tokens):
            reject(f"missing {what}")
        try:
            value = int(out_tokens[pos])
        except ValueError:
            reject(f"{what} is not an integer: '{out_tokens[pos]}'")
        pos += 1
        return value

    covered = set()

    def place(x, y, what):
        if not (1 <= x <= n and 1 <= y <= n):
            reject(f"{what} square ({x}, {y}) outside the board")
        if (x, y) in removed:
            reject(f"{what} covers removed square ({x}, {y})")
        if (x, y) in covered:
            reject(f"square ({x}, {y}) covered twice")
        covered.add((x, y))

    nh = next_int("Nh")
    if nh < 0:
        reject("negative Nh")
    for i in range(nh):
        x = next_int("horizontal die x")
        y = next_int("horizontal die y")
        place(x, y, f"horizontal die {i + 1}")
        place(x + 1, y, f"horizontal die {i + 1}")
    nv = next_int("Nv")
    if nv < 0:
        reject("negative Nv")
    for i in range(nv):
        x = next_int("vertical die x")
        y = next_int("vertical die y")
        place(x, y, f"vertical die {i + 1}")
        place(x, y + 1, f"vertical die {i + 1}")

    if pos != len(out_tokens):
        reject("trailing output after the tiling")
    if covered != free:
        missing = next(iter(free - covered))
        reject(f"square {missing} is not covered")

    sys.exit(0)


if __name__ == "__main__":
    main()
