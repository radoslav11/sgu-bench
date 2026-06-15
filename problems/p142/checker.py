"""Checker for p142: usage checker.py <in> <out> <ans>; exit 0 = accept.

The keyword length is deterministic and must equal the reference's. The
keyword itself may be any string over {a,b} of that length that does not
occur as a contiguous substring of the key-string.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    s = in_tokens[1]
    expected_len = int(ans_tokens[0])

    if len(out_tokens) != 2:
        reject(f"expected 2 tokens (length, keyword), got {len(out_tokens)}")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject("first token must be an integer length")
    keyword = out_tokens[1]

    if k != expected_len:
        reject(f"keyword length must be {expected_len}, got {k}")
    if len(keyword) != k:
        reject(f"declared length {k} but keyword has {len(keyword)} chars")
    if any(c not in "ab" for c in keyword):
        reject("keyword must use only 'a' and 'b'")
    if keyword in s:
        reject("keyword occurs in the key-string")

    sys.exit(0)


if __name__ == "__main__":
    main()
