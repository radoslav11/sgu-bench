"""Checker for p401: usage checker.py <in> <out> <ans>; exit 0 = accept.

The optimal distance must match the reference within 1e-6 relative or
absolute error. The certificate (crossing times t_1..t_N) is validated
against the input: t_i >= w_i/u, sum t_i <= T, and the distance the
times actually achieve must reach the reference optimum (small extra
slack absorbs printing/rounding noise near t_i = w_i/u where the
northward term has infinite derivative). The reference's own times are
never compared against.
"""

import math
import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    u = float(in_tokens[1])
    t_total = float(in_tokens[2])
    w = [float(in_tokens[3 + 2 * i]) for i in range(n)]

    if not out_tokens:
        reject("empty output")

    if ans_tokens[0] == "-1":
        if out_tokens[0] != "-1":
            reject(f"answer is -1, got '{out_tokens[0]}'")
        if len(out_tokens) > 1:
            reject("trailing output after -1")
        sys.exit(0)

    if out_tokens[0] == "-1":
        reject("claimed -1 but a solution exists")

    d_ref = float(ans_tokens[0])
    try:
        d_out = float(out_tokens[0])
    except ValueError:
        reject(f"distance is not a number: '{out_tokens[0]}'")
    if not math.isfinite(d_out):
        reject("distance is not finite")
    if abs(d_out - d_ref) > 1e-6 * max(1.0, abs(d_ref)):
        reject(f"distance {d_out} differs from optimum {d_ref}")

    if len(out_tokens) != 1 + n:
        reject(f"expected {n} times after the distance, got {len(out_tokens) - 1}")
    try:
        times = [float(x) for x in out_tokens[1:]]
    except ValueError:
        reject("times contain a non-number")
    if any(not math.isfinite(ti) for ti in times):
        reject("times contain a non-finite value")

    if sum(times) > t_total + 1e-6 * max(1.0, t_total):
        reject(f"times sum to {sum(times)} > available {t_total}")
    y_total = 0.0
    for i, ti in enumerate(times):
        if ti < w[i] / u - 1e-6:
            reject(f"river {i + 1}: time {ti} < minimal crossing time {w[i] / u}")
        v_i = float(in_tokens[4 + 2 * i])
        drift_sq = max(0.0, (u * ti) * (u * ti) - w[i] * w[i])
        y_total += v_i * ti + math.sqrt(drift_sq)

    d_cert = math.hypot(sum(w), y_total)
    if d_cert < d_ref - max(1e-4, 1e-6 * d_ref):
        reject(f"times achieve distance {d_cert}, optimum is {d_ref}")

    sys.exit(0)


if __name__ == "__main__":
    main()
