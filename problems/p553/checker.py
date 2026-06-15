"""Checker for p553 (Sultan's Pearls): checker.py <in> <out> <ans>.

The maximum stolen value s is the deterministic optimum, so it must equal
the reference's. The number of pearls p is NOT unique: different optimal
plans can reach the same maximum value with different move counts, so p is
treated as part of the certificate, not compared to the reference's p. The
H/T sequence is replayed against the input and accepted iff it is feasible
(each move legal, balance holds after every step), has length p, and
yields exactly value s. The reference's own p and sequence are never
required to match the submission's.

State while replaying: pearls lo..hi (inclusive, original indices) remain
on the string; the bottom m of them hang, the rest lie on the table.
- 'H': steal the bottom pearl (index hi), slide one table pearl into the
  hanging part. Requires length > m so m pearls still hang afterwards.
- 'T': steal the top table pearl (index lo); hanging part unchanged.
  Requires a table pearl to exist (length > m).
After every move:  sum(hanging weights) <= k * sum(table weights).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    idx = 0
    n = int(in_tokens[idx]); idx += 1
    m = int(in_tokens[idx]); idx += 1
    k = int(in_tokens[idx]); idx += 1
    w = [0] * n
    c = [0] * n
    for i in range(n):
        w[i] = int(in_tokens[idx]); idx += 1
        c[i] = int(in_tokens[idx]); idx += 1

    if len(ans_tokens) < 2:
        reject("reference answer malformed")
    ref_s = int(ans_tokens[1])

    if not out_tokens:
        reject("empty output")
    try:
        out_p = int(out_tokens[0])
        out_s = int(out_tokens[1])
    except (ValueError, IndexError):
        reject("first line must be two integers p and s")

    if out_s != ref_s:
        reject(f"wrong sum: expected {ref_s}, got {out_s}")

    if out_p == 0:
        # Sequence line may be absent or empty; any extra token is wrong.
        if len(out_tokens) > 2:
            reject("extra output after '0 0'")
        sys.exit(0)

    if len(out_tokens) < 3:
        reject("missing operation sequence")
    seq = out_tokens[2]
    if len(out_tokens) > 3:
        reject("unexpected extra tokens after the sequence")
    if len(seq) != out_p:
        reject(f"sequence length {len(seq)} != declared count {out_p}")
    if any(ch not in "HT" for ch in seq):
        reject("sequence contains characters other than 'H'/'T'")

    lo, hi = 0, n - 1
    prefW = [0] * (n + 1)
    for i in range(n):
        prefW[i + 1] = prefW[i] + w[i]

    def table_hang_weights():
        length = hi - lo + 1
        hang = prefW[hi + 1] - prefW[hi + 1 - m]
        table = prefW[hi + 1 - m] - prefW[lo]
        return table, hang, length

    total = 0
    for step, ch in enumerate(seq):
        length = hi - lo + 1
        if length <= m:
            reject(f"step {step + 1}: no table pearl left to take")
        if ch == "H":
            total += c[hi]
            hi -= 1
        else:
            total += c[lo]
            lo += 1
        table, hang, _ = table_hang_weights()
        if hang > k * table:
            reject(
                f"step {step + 1}: balance broken "
                f"(W_hang={hang} > {k}*W_table={k * table})"
            )

    if total != out_s:
        reject(f"sequence value {total} does not match declared sum {out_s}")

    sys.exit(0)


if __name__ == "__main__":
    main()
