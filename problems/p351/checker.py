"""Checker for p351: usage checker.py <in> <out> <ans>; exit 0 = accept.

The route is free-form ("if there are several solutions..." is implied by
the geometry: tangent points may be listed with either orientation, arcs
may be split, the chord may sit anywhere on the arc), so the output is
validated against the input instead of compared to the reference text:

- format: T parts (T <= 100), each "S|A x1 y1 x2 y2";
- the chain starts at A, ends at B, consecutive parts share endpoints;
- arc endpoints lie on the crater circle; an arc is read as the minor
  arc, matching the reference/sample convention (endpoints alone cannot
  encode more than half a turn);
- total length of segment portions strictly inside the crater is <= D;
- total route length must not exceed the reference optimum (parsed from
  <ans> with the same length rules) plus 1e-3.
"""

import math
import sys

JOIN_EPS = 1e-5
CIRCLE_EPS = 1e-4
FLY_EPS = 1e-4
LEN_EPS = 1e-3


def reject(message):
    print(message)
    sys.exit(1)


def parse_route(tokens, where):
    if not tokens:
        reject(f"{where}: empty")
    try:
        t = int(tokens[0])
    except ValueError:
        reject(f"{where}: first token is not an integer part count")
    if not (1 <= t <= 100):
        reject(f"{where}: part count {t} out of [1, 100]")
    if len(tokens) != 1 + 5 * t:
        reject(f"{where}: expected {1 + 5 * t} tokens for {t} parts, got {len(tokens)}")
    parts = []
    for i in range(t):
        chunk = tokens[1 + 5 * i : 6 + 5 * i]
        kind = chunk[0].upper()
        if kind not in ("S", "A"):
            reject(f"{where}: part {i + 1} type must be S or A, got '{chunk[0]}'")
        try:
            x1, y1, x2, y2 = map(float, chunk[1:])
        except ValueError:
            reject(f"{where}: part {i + 1} has non-numeric coordinates")
        if any(math.isnan(v) or math.isinf(v) for v in (x1, y1, x2, y2)):
            reject(f"{where}: part {i + 1} has nan/inf coordinates")
        parts.append((kind, (x1, y1), (x2, y2)))
    return parts


def measure(parts, A, B, C, R, where):
    # Returns (total_length, length_flying_inside_crater) or rejects.
    if math.hypot(parts[0][1][0] - A[0], parts[0][1][1] - A[1]) > JOIN_EPS:
        reject(f"{where}: route does not start at A")
    if math.hypot(parts[-1][2][0] - B[0], parts[-1][2][1] - B[1]) > JOIN_EPS:
        reject(f"{where}: route does not end at B")
    for i in range(len(parts) - 1):
        p = parts[i][2]
        q = parts[i + 1][1]
        if math.hypot(p[0] - q[0], p[1] - q[1]) > JOIN_EPS:
            reject(f"{where}: parts {i + 1} and {i + 2} do not connect")

    total = 0.0
    inside = 0.0
    r_in = R - 1e-6  # measuring radius: boundary contact is not flight
    for idx, (kind, p, q) in enumerate(parts, 1):
        if kind == "S":
            dx, dy = q[0] - p[0], q[1] - p[1]
            seg = math.hypot(dx, dy)
            total += seg
            if seg > 0:
                # Clip the segment against the (slightly shrunk) disc.
                fx, fy = p[0] - C[0], p[1] - C[1]
                a = dx * dx + dy * dy
                b = 2 * (fx * dx + fy * dy)
                c = fx * fx + fy * fy - r_in * r_in
                disc = b * b - 4 * a * c
                if disc > 0:
                    sq = math.sqrt(disc)
                    t1 = max(0.0, (-b - sq) / (2 * a))
                    t2 = min(1.0, (-b + sq) / (2 * a))
                    if t2 > t1:
                        inside += (t2 - t1) * seg
        else:
            d1 = math.hypot(p[0] - C[0], p[1] - C[1])
            d2 = math.hypot(q[0] - C[0], q[1] - C[1])
            if abs(d1 - R) > CIRCLE_EPS or abs(d2 - R) > CIRCLE_EPS:
                reject(f"{where}: part {idx} arc endpoint not on the crater circle")
            a1 = math.atan2(p[1] - C[1], p[0] - C[0])
            a2 = math.atan2(q[1] - C[1], q[0] - C[0])
            dphi = abs(a2 - a1)
            if dphi > math.pi:
                dphi = 2 * math.pi - dphi
            total += R * dphi
    return total, inside


def main():
    in_vals = [float(x) for x in open(sys.argv[1]).read().split()]
    ax, ay, bx, by, cx, cy, r, d = in_vals
    A, B, C = (ax, ay), (bx, by), (cx, cy)

    out_parts = parse_route(open(sys.argv[2]).read().split(), "output")
    ans_parts = parse_route(open(sys.argv[3]).read().split(), "answer")

    out_len, out_fly = measure(out_parts, A, B, C, r, "output")
    ans_len, _ = measure(ans_parts, A, B, C, r, "answer")

    if out_fly > d + FLY_EPS:
        reject(f"flight inside crater {out_fly:.9f} exceeds fuel D={d:g}")
    if out_len > ans_len + LEN_EPS:
        reject(f"route length {out_len:.9f} exceeds optimum {ans_len:.9f}")

    sys.exit(0)


if __name__ == "__main__":
    main()
