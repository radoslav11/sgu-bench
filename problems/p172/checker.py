"""Checker for p172: usage checker.py <in> <out> <ans>; exit 0 = accept.

The yes/no part is deterministic, so it must match the reference. A "yes"
must come with a certificate: a set of subjects (day one) such that no
pupil has both selected subjects on the same day. Any valid certificate
is accepted; the reference's own set is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected = ans_tokens[0].lower()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be yes/no, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "no":
        if len(out_tokens) > 1:
            reject("trailing output after 'no'")
        sys.exit(0)

    n, m = int(in_tokens[0]), int(in_tokens[1])
    pairs = in_tokens[2:]

    if len(out_tokens) < 2:
        reject("missing day-one count")
    k = int(out_tokens[1])
    day_one_list = [int(x) for x in out_tokens[2:]]
    if len(day_one_list) != k:
        reject(f"declared {k} subjects, listed {len(day_one_list)}")
    if len(set(day_one_list)) != k:
        reject("duplicate subjects in day-one list")
    if any(not (1 <= s <= n) for s in day_one_list):
        reject("subject out of range")

    day_one = set(day_one_list)
    for i in range(m):
        u, v = int(pairs[2 * i]), int(pairs[2 * i + 1])
        if (u in day_one) == (v in day_one):
            reject(f"pupil {i + 1} has both exams ({u}, {v}) on the same day")

    sys.exit(0)


if __name__ == "__main__":
    main()
