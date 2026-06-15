"""Checker for p548: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximum gold (or -1 when the Knight cannot marry the last princess) is
deterministic and must equal the reference value. When a solution exists
the output lists the dragon cells the Knight kills; any list is accepted as
long as:

  - every listed cell holds a dragon (cells 2..n-1),
  - walking 1..n in order, at each intermediate princess (cells 2..n-1) the
    number of dragons killed so far is strictly less than her beauty
    (otherwise the Knight marries early and never reaches cell n),
  - at the last cell (a princess) the number killed so far is >= her beauty,
  - the total gold collected equals the reference maximum.

The reference's own kill set is never required to match.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_lines = open(sys.argv[1]).read().split("\n")
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    idx = 0
    while idx < len(in_lines) and in_lines[idx].strip() == "":
        idx += 1
    n = int(in_lines[idx].split()[0])

    # cell[i] for i in 2..n: (type, value)
    cell_type = [None] * (n + 1)
    cell_val = [0] * (n + 1)
    line_no = idx + 1
    for i in range(2, n + 1):
        parts = in_lines[line_no].split()
        line_no += 1
        cell_type[i] = parts[0]
        cell_val[i] = int(parts[1])

    if not ans_tokens:
        reject("reference produced no answer")
    ref_first = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        got_gold = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")

    if ref_first == -1:
        if got_gold != -1:
            reject(f"reference says impossible (-1), got {got_gold}")
        sys.exit(0)

    if got_gold == -1:
        reject(f"a marriage is possible (gold {ref_first}), got -1")
    if got_gold != ref_first:
        reject(f"max gold is {ref_first}, got {got_gold}")

    # parse k and the kill list
    if len(out_tokens) < 2:
        reject("missing kill count")
    k = int(out_tokens[1])
    kills = [int(x) for x in out_tokens[2:]]
    if len(kills) != k:
        reject(f"declared {k} kills, listed {len(kills)}")
    if len(set(kills)) != k:
        reject("duplicate dragon cell in kill list")

    killset = set()
    for c in kills:
        if not (2 <= c <= n):
            reject(f"kill cell {c} out of range")
        if cell_type[c] != "d":
            reject(f"cell {c} does not hold a dragon")
        killset.add(c)

    gold = sum(cell_val[c] for c in killset)
    if gold != got_gold:
        reject(f"kills collect {gold} gold, declared {got_gold}")

    killed = 0
    for i in range(2, n + 1):
        if cell_type[i] == "d":
            if i in killset:
                killed += 1
        else:  # princess
            if i == n:
                if killed < cell_val[i]:
                    reject(
                        f"last princess beauty {cell_val[i]} but only "
                        f"{killed} dragons killed before cell {n}"
                    )
            else:
                if killed >= cell_val[i]:
                    reject(
                        f"princess at cell {i} (beauty {cell_val[i]}) marries "
                        f"the Knight early: {killed} dragons already killed"
                    )

    sys.exit(0)


if __name__ == "__main__":
    main()
