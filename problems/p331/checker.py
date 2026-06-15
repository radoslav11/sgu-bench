"""Checker for p331: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal travel time is deterministic up to 1e-6, so the claimed time
must not exceed the reference time plus 1e-6. The schedule itself is a
certificate: simulate it (driving distance is the exact integral of
b + a*sin(t + delta); lane changes take c*|x-y| with no movement and must
not overlap) and require the distance covered by the claimed finish time
to be at least d - 1e-6. Any valid schedule is accepted.
"""

import math
import sys

EPS = 1e-6


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    d_total = float(in_tokens[1])
    c = float(in_tokens[2])
    lanes = []
    for i in range(n):
        a = float(in_tokens[3 + 3 * i])
        b = float(in_tokens[4 + 3 * i])
        delta = float(in_tokens[5 + 3 * i])
        lanes.append((a, b, delta))

    ref_time = float(ans_tokens[0])

    if len(out_tokens) < 2:
        reject("output must contain the time and the number of changes")
    try:
        t_end = float(out_tokens[0])
        k = int(out_tokens[1])
    except ValueError:
        reject("first two tokens must be the time and an integer count")
    if not math.isfinite(t_end) or t_end < -EPS:
        reject(f"bad finish time {out_tokens[0]}")
    if k < 0 or k > 10**6:
        reject(f"number of changes {k} out of [0, 10^6]")
    if len(out_tokens) != 2 + 2 * k:
        reject(f"expected {2 + 2 * k} tokens for K={k}, got {len(out_tokens)}")

    changes = []
    for i in range(k):
        try:
            lane = int(out_tokens[2 + 2 * i])
            t = float(out_tokens[3 + 2 * i])
        except ValueError:
            reject(f"malformed change #{i + 1}")
        if not (1 <= lane <= n):
            reject(f"change #{i + 1}: lane {lane} out of range")
        if not math.isfinite(t):
            reject(f"change #{i + 1}: bad time")
        changes.append((lane, t))

    def segment_dist(lane, u, v):
        a, b, delta = lanes[lane - 1]
        return b * (v - u) - a * (math.cos(v + delta) - math.cos(u + delta))

    cur_lane = 1
    cur_time = 0.0  # moment from which we are driving on cur_lane
    dist = 0.0
    for i, (lane, t) in enumerate(changes):
        if t < cur_time - EPS:
            reject(
                f"change #{i + 1} at t={t} starts before previous "
                f"activity ends at {cur_time}"
            )
        t_eff = max(t, cur_time)
        if t_eff > t_end + EPS:
            break
        dist += segment_dist(cur_lane, cur_time, min(t_eff, t_end))
        cur_time = t_eff + c * abs(lane - cur_lane)
        cur_lane = lane

    if t_end > cur_time:
        dist += segment_dist(cur_lane, cur_time, t_end)

    if dist < d_total - EPS:
        reject(
            f"schedule covers {dist:.9f} by t={t_end}, needs {d_total}"
        )
    if t_end > ref_time + EPS:
        reject(f"finish time {t_end} worse than optimal {ref_time}")

    sys.exit(0)


if __name__ == "__main__":
    main()
