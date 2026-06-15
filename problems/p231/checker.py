"""Checker for p231: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of valid pairs is unique, but the statement fixes no output
order, so the output's pair multiset must equal the reference's pair
multiset (each pair printed as "A B" with A <= B), preceded by the count.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_pairs(tokens, label):
    if not tokens:
        reject(f"{label}: empty")
    try:
        cnt = int(tokens[0])
        rest = [int(x) for x in tokens[1:]]
    except ValueError:
        reject(f"{label}: non-integer token")
    if cnt < 0 or len(rest) != 2 * cnt:
        reject(f"{label}: declared {cnt} pairs, found {len(rest)} numbers")
    return sorted((rest[2 * i], rest[2 * i + 1]) for i in range(cnt))


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    ans_pairs = parse_pairs(ans_tokens, "answer file")
    out_pairs = parse_pairs(out_tokens, "output")

    if len(out_pairs) != len(ans_pairs):
        reject(f"expected {len(ans_pairs)} pairs, got {len(out_pairs)}")
    if out_pairs != ans_pairs:
        for got, exp in zip(out_pairs, ans_pairs):
            if got != exp:
                reject(f"pair {got} should be {exp}")
    sys.exit(0)


if __name__ == "__main__":
    main()
