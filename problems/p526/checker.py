"""Checker for p526: usage checker.py <in> <out> <ans>; exit 0 = accept.

Feasibility (-1 vs a route) is deterministic and must match the reference.
When a route exists the answer is a certificate: any route reaching Jasmin
in the minimum time is accepted. We validate it directly against the input:
each instruction must respect the speed bound, the trajectory must start at
0, must never be strictly inside a stone interval when that stone lands
(before arrival), the final position must equal G, and the total running
time must equal the reference's minimum time. The reference's own route is
never required to match.
"""

import sys

POS_EPS = 1e-4
TIME_EPS = 1e-6


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    v = int(in_tokens[0])
    g = int(in_tokens[1])
    n = int(in_tokens[2])
    stones = []
    idx = 3
    for _ in range(n):
        x1 = int(in_tokens[idx]); x2 = int(in_tokens[idx + 1])
        t = int(in_tokens[idx + 2]); idx += 3
        stones.append((x1, x2, t))

    ref_infeasible = (ans_tokens[0] == "-1")

    if not out_tokens:
        reject("empty output")

    out_infeasible = (out_tokens[0] == "-1")
    if ref_infeasible != out_infeasible:
        reject(
            f"feasibility mismatch: reference says "
            f"{'-1' if ref_infeasible else 'route'}, "
            f"output says {'-1' if out_infeasible else 'route'}"
        )

    if ref_infeasible:
        if len(out_tokens) != 1:
            reject("expected exactly '-1'")
        sys.exit(0)

    # Reference minimum time = total duration of the reference route.
    ref_k = int(ans_tokens[0])
    ref_total = 0.0
    pos = 1
    for _ in range(ref_k):
        pos += 1  # speed
        ref_total += float(ans_tokens[pos])
        pos += 1

    # Parse participant route.
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject("first token must be -1 or k")
    if k < 0 or k > 10000:
        reject(f"k={k} out of range [0,10000]")
    if len(out_tokens) != 1 + 2 * k:
        reject(f"expected {1 + 2 * k} tokens, got {len(out_tokens)}")

    instr = []
    p = 1
    for _ in range(k):
        try:
            w = float(out_tokens[p]); t = float(out_tokens[p + 1])
        except ValueError:
            reject("non-numeric instruction")
        p += 2
        if t <= 0:
            reject(f"instruction time must be positive, got {t}")
        if abs(w) > v + 1e-6:
            reject(f"speed {w} exceeds max {v}")
        instr.append((w, t))

    total_time = sum(t for _, t in instr)
    if abs(total_time - ref_total) > 1e-4 * max(1.0, abs(ref_total)) + 1e-6:
        reject(
            f"total time {total_time} differs from minimum {ref_total}"
        )

    # Build breakpoints (time, position) of the piecewise-linear trajectory.
    bp_t = [0.0]
    bp_x = [0.0]
    cx = 0.0
    ct = 0.0
    for w, t in instr:
        ct += t
        cx += w * t
        bp_t.append(ct)
        bp_x.append(cx)

    if abs(cx - g) > POS_EPS:
        reject(f"final position {cx} != G {g}")

    def position_at(tq):
        # Locate the segment containing time tq.
        if tq <= bp_t[0]:
            return bp_x[0]
        if tq >= bp_t[-1]:
            return bp_x[-1]
        lo, hi = 0, len(bp_t) - 1
        while lo + 1 < hi:
            mid = (lo + hi) // 2
            if bp_t[mid] <= tq:
                lo = mid
            else:
                hi = mid
        t0, t1 = bp_t[lo], bp_t[lo + 1]
        x0, x1 = bp_x[lo], bp_x[lo + 1]
        if t1 == t0:
            return x0
        frac = (tq - t0) / (t1 - t0)
        return x0 + (x1 - x0) * frac

    # Check every stone that lands strictly before arrival.
    for (sx1, sx2, st) in stones:
        if sx1 >= sx2:
            continue
        if st >= total_time - TIME_EPS:
            continue
        px = position_at(float(st))
        if px > sx1 + POS_EPS and px < sx2 - POS_EPS:
            reject(
                f"killed by stone ({sx1},{sx2}) at t={st}: position {px}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
