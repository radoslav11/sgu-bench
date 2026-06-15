"""Checker for p478: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility is deterministic, so "ERROR" must match the reference. A
feasible schedule is n pairs (boys joined, girls left) per note; it is
validated against the input: totals within a and b, and the group size
(boys so far + remaining girls) must equal each recorded note.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    a, b = int(in_tokens[0]), int(in_tokens[1])
    n = int(in_tokens[2])
    notes = [int(x) for x in in_tokens[3:3 + n]]

    infeasible = ans_tokens and ans_tokens[0].upper() == "ERROR"

    if not out_tokens:
        reject("empty output")
    if out_tokens[0].upper() == "ERROR":
        if len(out_tokens) > 1:
            reject("trailing output after 'ERROR'")
        if not infeasible:
            reject("said 'ERROR' but a schedule exists")
        sys.exit(0)
    if infeasible:
        reject("no schedule exists, but output gives one")

    if len(out_tokens) != 2 * n:
        reject(f"expected {2 * n} integers, got {len(out_tokens)} tokens")
    try:
        vals = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer tokens in schedule")

    boys = 0
    girls = b
    for i in range(n):
        joined, left = vals[2 * i], vals[2 * i + 1]
        if joined < 0 or left < 0:
            reject(f"negative value at note {i + 1}")
        boys += joined
        girls -= left
        if boys > a:
            reject(f"more than {a} boys joined by note {i + 1}")
        if girls < 0:
            reject(f"more than {b} girls left by note {i + 1}")
        if boys + girls != notes[i]:
            reject(
                f"note {i + 1}: group has {boys + girls}, "
                f"teacher wrote {notes[i]}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
