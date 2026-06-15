"""Checker for p304: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximal number of cured teeth S is deterministic and must match the
reference. The tooth list is a certificate: S distinct valid indices
whose total cost (tooth costs plus anesthesia of every gum that has a
cured tooth) must fit in the budget P. Any such set is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()
    ans_tok = open(sys.argv[3]).read().split()

    n, k, p = int(in_tok[0]), int(in_tok[1]), int(in_tok[2])
    b = [int(t) for t in in_tok[3:3 + k]]
    teeth = []
    for i in range(n):
        a = int(in_tok[3 + k + 2 * i])
        c = int(in_tok[4 + k + 2 * i])
        teeth.append((a, c))

    expected_s = int(ans_tok[0])

    if not out_tok:
        reject("empty output")
    try:
        s = int(out_tok[0])
        idx = [int(t) for t in out_tok[1:]]
    except ValueError:
        reject("non-integer token in output")
    if s != expected_s:
        reject(f"maximal number of teeth is {expected_s}, got {s}")
    if len(idx) != s:
        reject(f"declared {s} teeth, listed {len(idx)}")
    if len(set(idx)) != s:
        reject("duplicate tooth indices")
    if any(not (1 <= i <= n) for i in idx):
        reject("tooth index out of range")

    cost = sum(teeth[i - 1][0] for i in idx)
    cost += sum(b[g - 1] for g in {teeth[i - 1][1] for i in idx})
    if cost > p:
        reject(f"total cost {cost} exceeds budget {p}")

    sys.exit(0)


if __name__ == "__main__":
    main()
