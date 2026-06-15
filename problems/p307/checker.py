"""Checker for p307: usage checker.py <in> <out> <ans>; exit 0 = accept.

Whether a key exists is deterministic: if the reference printed CORRUPT
the output must too. Otherwise the output must be any H x W matrix of
0/1 characters whose 2x2 sums reproduce the fingerprint from the input;
the reference's own key is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_lines = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_first = open(sys.argv[3]).read().split()[0]

    if not out_tokens:
        reject("empty output")

    if ans_first == "CORRUPT":
        if out_tokens[0] != "CORRUPT":
            reject("fingerprint is corrupt, but output claims a key")
        if len(out_tokens) > 1:
            reject("trailing output after CORRUPT")
        sys.exit(0)

    if out_tokens[0] == "CORRUPT":
        reject("a key exists, but output claims CORRUPT")

    h, w = int(in_lines[0]), int(in_lines[1])
    fp = in_lines[2:]
    if len(fp) != h - 1:
        reject("internal: bad input parse")

    if len(out_tokens) != h:
        reject(f"expected {h} key rows, got {len(out_tokens)}")
    for row in out_tokens:
        if len(row) != w or any(ch not in "01" for ch in row):
            reject(f"bad key row '{row[:40]}'")

    key = [[int(ch) for ch in row] for row in out_tokens]
    for i in range(h - 1):
        for j in range(w - 1):
            s = key[i][j] + key[i + 1][j] + key[i][j + 1] + key[i + 1][j + 1]
            if s != int(fp[i][j]):
                reject(f"2x2 sum at ({i},{j}) is {s}, fingerprint says {fp[i][j]}")
    sys.exit(0)


if __name__ == "__main__":
    main()
