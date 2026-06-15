"""Checker for p242: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO part is deterministic and must match the reference. A "YES"
must come with K student lists: list j holds at least two distinct
students, each of whom named university j in his preference list, and no
student appears in more than one list. Any valid assignment is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    n, k = int(in_tokens[pos]), int(in_tokens[pos + 1])
    pos += 2
    prefs = []
    for _ in range(n):
        cnt = int(in_tokens[pos])
        pos += 1
        prefs.append(set(int(x) for x in in_tokens[pos:pos + cnt]))
        pos += cnt

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

    used = set()
    idx = 1
    for j in range(1, k + 1):
        if idx >= len(out_tokens):
            reject(f"missing list for university {j}")
        try:
            cnt = int(out_tokens[idx])
        except ValueError:
            reject(f"university {j}: count is not an integer")
        idx += 1
        if cnt < 2:
            reject(f"university {j}: needs at least 2 students, got {cnt}")
        if idx + cnt > len(out_tokens):
            reject(f"university {j}: list truncated")
        for t in out_tokens[idx:idx + cnt]:
            try:
                s = int(t)
            except ValueError:
                reject(f"university {j}: student id '{t}' is not an integer")
            if not (1 <= s <= n):
                reject(f"university {j}: student {s} out of range")
            if s in used:
                reject(f"student {s} assigned to more than one university")
            if j not in prefs[s - 1]:
                reject(f"student {s} does not list university {j}")
            used.add(s)
        idx += cnt

    if idx != len(out_tokens):
        reject("trailing output after the last university list")

    sys.exit(0)


if __name__ == "__main__":
    main()
