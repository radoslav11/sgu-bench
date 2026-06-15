"""Checker for p531 (Bonnie and Clyde): usage checker.py <in> <out> <ans>.

The optimal sum is unique, but the pair of indices achieving it is not, so
the contestant may print any valid pair. We validate the contestant's pair
against the INPUT (distinct banks, distance >= d) and require its money sum
to equal the reference's optimal sum. The "-1 -1" no-solution case must
match the reference exactly.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_pair(tokens, label):
    if len(tokens) < 2:
        reject(f"{label}: expected two integers, got {tokens}")
    try:
        a = int(tokens[0])
        b = int(tokens[1])
    except ValueError:
        reject(f"{label}: non-integer output {tokens[:2]}")

    return a, b


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    d = int(in_tokens[1])
    xs = []
    ws = []
    idx = 2
    for _ in range(n):
        xs.append(int(in_tokens[idx]))
        ws.append(int(in_tokens[idx + 1]))
        idx += 2

    ra, rb = parse_pair(ans_tokens, "reference")
    oa, ob = parse_pair(out_tokens, "output")

    ref_none = (ra == -1 and rb == -1)
    out_none = (oa == -1 and ob == -1)

    if ref_none:
        if not out_none:
            reject(
                f"reference reports no valid pair, output claims pair "
                f"({oa}, {ob})"
            )
        sys.exit(0)

    # Reference found a valid pair; compute its sum as the optimum.
    if not (1 <= ra <= n and 1 <= rb <= n):
        reject("reference indices out of range (package bug)")
    ref_sum = ws[ra - 1] + ws[rb - 1]

    if out_none:
        reject(
            f"output reports no pair, but an optimal pair with sum "
            f"{ref_sum} exists"
        )

    if oa == ob:
        reject(f"the two bank indices must differ, got {oa} {ob}")
    if not (1 <= oa <= n) or not (1 <= ob <= n):
        reject(f"bank index out of range [1,{n}]: {oa} {ob}")

    if abs(xs[oa - 1] - xs[ob - 1]) < d:
        reject(
            f"distance between banks {oa} and {ob} is "
            f"{abs(xs[oa - 1] - xs[ob - 1])} < d={d}"
        )

    out_sum = ws[oa - 1] + ws[ob - 1]
    if out_sum != ref_sum:
        reject(f"sum {out_sum} is not optimal (reference sum {ref_sum})")

    sys.exit(0)


if __name__ == "__main__":
    main()
