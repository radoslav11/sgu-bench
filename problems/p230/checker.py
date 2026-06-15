"""Checker for p230: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility is deterministic, so "No solution" must match the reference.
Otherwise the output must be a permutation of 1..N where every weighing
pair (P, Q) satisfies coin[P] < coin[Q]; any such permutation is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_text = open(sys.argv[3]).read()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    pairs = in_tokens[2:]

    expected_no = ans_text.split()[:2] == ["No", "solution"]
    out_tokens = out_text.split()
    if not out_tokens:
        reject("empty output")

    got_no = " ".join(out_tokens).lower().startswith("no solution")
    if expected_no != got_no:
        reject(
            "expected 'No solution'" if expected_no
            else "said 'No solution' but an assignment exists"
        )
    if got_no:
        if len(out_tokens) != 2:
            reject("extra output after 'No solution'")
        sys.exit(0)

    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")
    try:
        coin = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer token in output")
    if sorted(coin) != list(range(1, n + 1)):
        reject("output is not a permutation of 1..N")

    for i in range(m):
        p, q = int(pairs[2 * i]), int(pairs[2 * i + 1])
        if coin[p - 1] >= coin[q - 1]:
            reject(
                f"weighing {i + 1}: box {p} (coin {coin[p - 1]}) "
                f"not lighter than box {q} (coin {coin[q - 1]})"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
