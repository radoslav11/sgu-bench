"""Checker for p279: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility (YES/NO) is deterministic and must match the reference. A
"YES" must come with a bipermutation: each of i and -i (= i') exactly
once, every -i before i, the order of negative values equal to the order
of positive values, and a(i) = #{j > i : i before j} + #{j < i : i
before -j} matching the input. Any valid bipermutation is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    a = [int(x) for x in in_tokens[1:1 + n]]

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    seq_tokens = out_tokens[1:]
    if len(seq_tokens) != 2 * n:
        reject(f"expected {2 * n} objects, got {len(seq_tokens)}")
    try:
        seq = [int(x) for x in seq_tokens]
    except ValueError:
        reject("non-integer object in sequence")

    if any(v == 0 or abs(v) > n for v in seq):
        reject("object out of range")
    if len(set(seq)) != 2 * n:
        reject("objects are not distinct")

    pos_unprimed = [0] * (n + 1)
    pos_primed = [0] * (n + 1)
    primed_order = []
    unprimed_order = []
    for idx, v in enumerate(seq):
        if v > 0:
            pos_unprimed[v] = idx
            unprimed_order.append(v)
        else:
            pos_primed[-v] = idx
            primed_order.append(-v)

    if primed_order != unprimed_order:
        reject("primed objects are not in the same order as unprimed ones")
    for i in range(1, n + 1):
        if pos_primed[i] > pos_unprimed[i]:
            reject(f"{i}' appears after {i}")

    for i in range(1, n + 1):
        cnt = 0
        pi = pos_unprimed[i]
        for j in range(i + 1, n + 1):
            if pi < pos_unprimed[j]:
                cnt += 1
        for j in range(1, i):
            if pi < pos_primed[j]:
                cnt += 1
        if cnt != a[i - 1]:
            reject(f"a({i}) is {cnt}, expected {a[i - 1]}")

    sys.exit(0)


if __name__ == "__main__":
    main()
