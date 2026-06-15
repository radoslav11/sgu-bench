"""Checker for p431 (Wildcards): checker.py <in> <out> <ans>.

Feasibility is deterministic: either a pattern exists or the answer is
"OOPS". So the reference's "OOPS" verdict is authoritative and the
submission must agree with it.

When a pattern is produced it is validated against the input directly:
it may use only lowercase letters, '*' and '?', is at most 100 characters
long, must match every one of the n "to match" names and none of the m
"not to match" names (matching uses standard glob semantics: '*' matches
any letter sequence including empty, '?' matches exactly one letter). Any
pattern meeting these requirements is accepted; the reference's own
pattern is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def glob_match(pattern, name):
    # Dynamic programming over (pattern index, name index).
    lp, ln = len(pattern), len(name)
    # dp[j] = pattern[0:i] can match name[0:j]; rolling over i.
    prev = [False] * (ln + 1)
    prev[0] = True
    for i in range(1, lp + 1):
        c = pattern[i - 1]
        cur = [False] * (ln + 1)
        if c == '*':
            cur[0] = prev[0]
            for j in range(1, ln + 1):
                cur[j] = cur[j - 1] or prev[j]
        elif c == '?':
            for j in range(1, ln + 1):
                cur[j] = prev[j - 1]
        else:
            for j in range(1, ln + 1):
                cur[j] = prev[j - 1] and name[j - 1] == c
        prev = cur
    return prev[ln]


def main():
    in_tokens = open(sys.argv[1]).read().split()
    n = int(in_tokens[0])
    m = int(in_tokens[1])
    names = in_tokens[2:2 + n + m]
    to_match = names[:n]
    to_avoid = names[n:n + m]

    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    ans_oops = bool(ans_tokens) and ans_tokens[0] == "OOPS"

    if not out_tokens:
        reject("empty output")
    pattern = out_tokens[0]
    if len(out_tokens) > 1:
        reject("output must be a single token")

    if pattern == "OOPS":
        if not ans_oops:
            reject("answered OOPS but a valid pattern exists")
        sys.exit(0)

    # A concrete pattern was given; the reference must not have said OOPS.
    if ans_oops:
        reject("produced a pattern but the instance is infeasible (OOPS)")

    if len(pattern) > 100:
        reject(f"pattern length {len(pattern)} exceeds 100")
    for ch in pattern:
        if not (ch.islower() and ch.isalpha()) and ch not in "*?":
            reject(f"illegal character '{ch}' in pattern")

    for name in to_match:
        if not glob_match(pattern, name):
            reject(f"pattern fails to match required name '{name}'")
    for name in to_avoid:
        if glob_match(pattern, name):
            reject(f"pattern wrongly matches forbidden name '{name}'")

    sys.exit(0)


if __name__ == "__main__":
    main()
