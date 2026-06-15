"""Checker for p305: usage checker.py <in> <out> <ans>; exit 0 = accept.

The output must be N distinct numbers in [1, M]. The objective is the
count of progressions i whose assigned number x satisfies x = Ai*K + Bi
for some integer K >= 0; it must be at least the reference's count. Any
assignment achieving that is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def member(a, b, x):
    if a == 0:
        return x == b
    d = x - b
    return d % a == 0 and d // a >= 0


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    m = int(in_tokens[1])
    prog = []
    for i in range(n):
        a = int(in_tokens[2 + 2 * i])
        b = int(in_tokens[3 + 2 * i])
        prog.append((a, b))

    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")
    try:
        out = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    if any(not (1 <= x <= m) for x in out):
        reject("assigned number out of [1, M]")
    if len(set(out)) != n:
        reject("assigned numbers are not unique")

    ans = [int(x) for x in ans_tokens]
    ref_score = sum(member(a, b, x) for (a, b), x in zip(prog, ans))
    out_score = sum(member(a, b, x) for (a, b), x in zip(prog, out))

    if out_score < ref_score:
        reject(f"only {out_score} progressions satisfied, optimum is {ref_score}")

    sys.exit(0)


if __name__ == "__main__":
    main()
