"""Checker for p260: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic: if the reference printed -1, the output
must be -1. Otherwise the output is L followed by L cell numbers;
clicking cell i toggles the colors of cells in its associated set
(toggles commute, repeats allowed). The final coloring must be
monochrome (all 0 or all 1). The reference's click list is never
compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    n = int(in_tokens[pos]); pos += 1
    groups = []
    for _ in range(n):
        k = int(in_tokens[pos]); pos += 1
        groups.append([int(x) for x in in_tokens[pos:pos + k]])
        pos += k
    colors = [int(x) for x in in_tokens[pos:pos + n]]

    ref_impossible = ans_tokens[0] == "-1"

    if not out_tokens:
        reject("empty output")
    if out_tokens[0] == "-1":
        if not ref_impossible:
            reject("claimed -1 but a solution exists")
        if len(out_tokens) > 1:
            reject("trailing output after -1")
        sys.exit(0)
    if ref_impossible:
        reject("printed clicks but puzzle is unsolvable")

    try:
        l = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be L or -1, got '{out_tokens[0]}'")
    if l < 0:
        reject(f"negative click count {l}")
    if l > 10**6:
        reject(f"click count {l} unreasonably large")
    if len(out_tokens) != 1 + l:
        reject(f"declared {l} clicks, found {len(out_tokens) - 1} numbers")

    click_parity = [0] * (n + 1)
    for tok in out_tokens[1:]:
        try:
            c = int(tok)
        except ValueError:
            reject(f"non-integer click '{tok}'")
        if not (1 <= c <= n):
            reject(f"click {c} out of range 1..{n}")
        click_parity[c] ^= 1

    final = colors[:]
    for i in range(n):
        if click_parity[i + 1]:
            for cell in groups[i]:
                final[cell - 1] ^= 1

    if any(c != final[0] for c in final):
        reject("final coloring is not monochrome")

    sys.exit(0)


if __name__ == "__main__":
    main()
