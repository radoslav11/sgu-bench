"""Checker for p469: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any center point is accepted whose achievable ghost radius (distance to
the nearest wall and to every inflated old ghost) is within 1 mm of the
radius achievable at the reference's point. The coordinates themselves
are never compared.
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

    w, h, d = (float(t) for t in in_tokens[:3])
    n = int(in_tokens[3])
    ghosts = []
    for i in range(n):
        x, y, z, r = (float(t) for t in in_tokens[4 + 4 * i:8 + 4 * i])
        ghosts.append((x, y, z, r))

    def radius_at(p):
        px, py, pz = p
        best = min(px, w - px, py, h - py, pz, d - pz)
        for x, y, z, r in ghosts:
            dist = math.sqrt((px - x) ** 2 + (py - y) ** 2 + (pz - z) ** 2)
            best = min(best, dist - r)
        return best

    if len(out_tokens) != 3:
        reject(f"expected 3 numbers, got {len(out_tokens)} tokens")
    try:
        out_p = tuple(float(t) for t in out_tokens)
    except ValueError:
        reject("non-numeric coordinate in output")
    for c in out_p:
        if not math.isfinite(c):
            reject("non-finite coordinate in output")

    ans_p = tuple(float(t) for t in ans_tokens[:3])

    r_out = radius_at(out_p)
    r_ans = radius_at(ans_p)
    if r_out < r_ans - 1e-3:
        reject(f"achievable radius {r_out:.6f} < optimal {r_ans:.6f} - 1mm")

    sys.exit(0)


if __name__ == "__main__":
    main()
