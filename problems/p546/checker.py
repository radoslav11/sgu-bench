"""Checker for p546: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum number of replacements t (or -1 when impossible) is
deterministic and must equal the reference value. When t >= 0 the second
output line is a certificate password; any password is accepted as long as
it has length n, exactly a '0's and b '1's (rest '2'), and differs from the
original in exactly t positions. The reference's own password is never
required to match.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, a, b = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    original = in_tokens[3] if len(in_tokens) > 3 else ""

    if not ans_tokens:
        reject("reference produced no answer")
    ref_t = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        got_t = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")

    if got_t != ref_t:
        reject(f"min replacements is {ref_t}, got {got_t}")

    if ref_t == -1:
        sys.exit(0)

    if len(out_tokens) < 2:
        reject("missing password line")
    password = out_tokens[1]

    if len(password) != n:
        reject(f"password length {len(password)} != n={n}")
    if any(c not in "012" for c in password):
        reject("password contains a non-ternary character")

    cnt0 = password.count("0")
    cnt1 = password.count("1")
    if cnt0 != a:
        reject(f"password has {cnt0} zeros, need {a}")
    if cnt1 != b:
        reject(f"password has {cnt1} ones, need {b}")

    diff = sum(1 for x, y in zip(original, password) if x != y)
    if diff != got_t:
        reject(f"password differs in {diff} positions, declared {got_t}")

    sys.exit(0)


if __name__ == "__main__":
    main()
