"""Checker for p257: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility is deterministic, so "no solution" must match the reference.
A claimed distribution is any string of N letters P/O/S such that every
creditor receives, by his own estimates (S=1, B=2 burcrystals), at least
his debt. The reference's own distribution is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_text = open(sys.argv[3]).read()

    debts = [int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])]
    n = int(in_tokens[3])
    crystals = in_tokens[4:4 + n]

    ref_no_solution = "no solution" in ans_text.lower()
    out_tokens = out_text.split()
    if not out_tokens:
        reject("empty output")

    got_no_solution = "no" in (t.lower() for t in out_tokens[:1])
    if got_no_solution:
        if not ref_no_solution:
            reject("claimed 'no solution' but a distribution exists")
        norm = " ".join(out_text.lower().split())
        if norm != "no solution":
            reject(f"expected exactly 'no solution', got '{norm[:40]}'")
        sys.exit(0)

    if ref_no_solution:
        reject("printed a distribution but there is no solution")

    if len(out_tokens) != 1:
        reject("distribution must be a single string of N letters")
    dist = out_tokens[0].upper()
    if len(dist) != n:
        reject(f"distribution length {len(dist)}, expected {n}")

    received = {"P": 0, "O": 0, "S": 0}
    index = {"P": 0, "O": 1, "S": 2}
    for i, who in enumerate(dist):
        if who not in index:
            reject(f"invalid letter '{who}' at position {i + 1}")
        received[who] += 2 if crystals[i][index[who]] == "B" else 1

    for name, j in index.items():
        if received[name] < debts[j]:
            reject(f"creditor {name} receives {received[name]} < debt {debts[j]}")

    sys.exit(0)


if __name__ == "__main__":
    main()
