"""Checker for p293: usage checker.py <in> <out> <ans>; exit 0 = accept.

The Qc/He verdict is deterministic, so it must match the reference. A
"Qc" answer must come with 2n-1 action lines that are simulated: He
appends letter k, then Qc's turn k either passes (0 0) or swaps two
positions in [1, k]; after every odd Qc turn the fence must read as a
palindrome. Any valid action sequence is accepted.
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
    s = in_tokens[1]
    m = 2 * n - 1

    expected = ans_tokens[0].lower()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    if got not in ("qc", "he"):
        reject(f"first token must be Qc/He, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{ans_tokens[0]}', got '{out_tokens[0]}'")
    if got == "he":
        if len(out_tokens) > 1:
            reject("trailing output after 'He'")
        sys.exit(0)

    rest = out_tokens[1:]
    if len(rest) != 2 * m:
        reject(f"expected {2 * m} numbers after 'Qc', got {len(rest)}")
    try:
        moves = [int(x) for x in rest]
    except ValueError:
        reject("non-integer move token")

    fence = []
    for k in range(1, m + 1):
        fence.append(s[k - 1])
        a, b = moves[2 * (k - 1)], moves[2 * (k - 1) + 1]
        if a == 0 and b == 0:
            pass
        elif 1 <= a <= k and 1 <= b <= k:
            fence[a - 1], fence[b - 1] = fence[b - 1], fence[a - 1]
        else:
            reject(f"turn {k}: positions {a} {b} out of range [1, {k}]")

        if k % 2 == 1 and fence != fence[::-1]:
            reject(f"after odd turn {k} the fence is not a palindrome")

    sys.exit(0)


if __name__ == "__main__":
    main()
