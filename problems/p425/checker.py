"""Checker for p425: usage checker.py <in> <out> <ans>; exit 0 = accept.

The Yes/No verdict must match the reference. On "Yes" the function is a
certificate: it must list every distinct matrix value exactly once as
"key -> value" with keys in increasing order, all values in [0, 50],
and applying it must keep every row below the first different from the
mapped first row in at least one column.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_first = open(sys.argv[3]).read().split()[0].lower()

    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be Yes/No, got '{out_tokens[0]}'")
    if got != ans_first:
        reject(f"verdict is '{ans_first}', got '{got}'")
    if got == "no":
        if len(out_tokens) > 1:
            reject("trailing output after 'No'")
        sys.exit(0)

    n, m = int(in_tokens[0]), int(in_tokens[1])
    flat = [int(x) for x in in_tokens[2:2 + n * m]]
    distinct = sorted(set(flat))

    body = out_tokens[1:]
    if len(body) != 3 * len(distinct):
        reject(
            f"expected {len(distinct)} 'key -> value' triples, "
            f"got {len(body)} tokens"
        )

    func = {}
    prev_key = None
    for i in range(len(distinct)):
        key_tok, arrow, val_tok = body[3 * i:3 * i + 3]
        if arrow != "->":
            reject(f"triple {i + 1}: expected '->', got '{arrow}'")
        try:
            key = int(key_tok)
            val = int(val_tok)
        except ValueError:
            reject(f"triple {i + 1}: non-integer key or value")
        if key != distinct[i]:
            reject(f"triple {i + 1}: expected key {distinct[i]}, got {key}")
        if prev_key is not None and key <= prev_key:
            reject("keys are not in increasing order")
        prev_key = key
        if not (0 <= val <= 50):
            reject(f"value {val} for key {key} is out of [0, 50]")
        func[key] = val

    mapped_first = [func[v] for v in flat[:m]]
    for i in range(1, n):
        row = flat[i * m:(i + 1) * m]
        if all(func[row[j]] == mapped_first[j] for j in range(m)):
            reject(f"mapped row {i + 1} equals the mapped first row")

    sys.exit(0)


if __name__ == "__main__":
    main()
