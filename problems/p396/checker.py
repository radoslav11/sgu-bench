"""Checker for p396: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum energy is deterministic and must match the reference. The
position sequence is a certificate: it is accepted iff it is achievable
by the rules and some assignment of presses to beats realizes exactly
the optimal energy. Feasibility is decided with a small DP over which
leg pressed at the previous beat (left / right / none / both), since
move costs depend on that.
"""

import sys

INF = 10**18
IDX = {"L": 0, "U": 1, "R": 2, "D": 3}


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    song = in_tokens[1]
    best = int(ans_tokens[0])

    if len(out_tokens) != n + 1:
        reject(f"expected {n + 1} tokens, got {len(out_tokens)}")
    try:
        claimed = int(out_tokens[0])
    except ValueError:
        reject(f"energy must be an integer, got '{out_tokens[0]}'")
    if claimed != best:
        reject(f"minimum energy is {best}, got {claimed}")

    positions = []
    for i, tok in enumerate(out_tokens[1:]):
        if len(tok) != 2 or tok[0] not in IDX or tok[1] not in IDX:
            reject(f"beat {i + 1}: bad position token '{tok}'")
        l, r = IDX[tok[0]], IDX[tok[1]]
        if l == r:
            reject(f"beat {i + 1}: both legs on the same button")
        if l == 2 and r == 0:
            reject(f"beat {i + 1}: illegal crossed position RL")
        positions.append((l, r))

    # dp[last] = min energy so far; last: 0 left pressed, 1 right, 2 none,
    # 3 both. Start: left on L(0), right on R(2), nothing pressed.
    dp = [INF, INF, 0, INF]
    pl, pr = 0, 2
    for i in range(n):
        req = -1 if song[i] == "N" else IDX[song[i]]
        l, r = positions[i]
        ndp = [INF, INF, INF, INF]

        def upd(nlast, cost_by_last):
            for last in range(4):
                if dp[last] < INF:
                    c = dp[last] + cost_by_last(last)
                    if c < ndp[nlast]:
                        ndp[nlast] = c

        if l == pl and r == pr:
            if req == -1:
                upd(2, lambda last: 0)
            if req == -1 or l == req:
                upd(0, lambda last: 1)
            if req == -1 or r == req:
                upd(1, lambda last: 1)
        if r == pr and (req == -1 or l == req):
            upd(0, lambda last: 9 if last in (0, 3) else 3)
        if l == pl and (req == -1 or r == req):
            upd(1, lambda last: 9 if last in (1, 3) else 3)
        if req == -1 or req in (l, r):
            upd(3, lambda last: 10)

        if min(ndp) >= INF:
            reject(f"beat {i + 1}: no legal action reaches position "
                   f"{out_tokens[1 + i]}")
        dp = ndp
        pl, pr = l, r

    achieved = min(dp)
    if achieved != best:
        reject(f"sequence costs {achieved}, optimum is {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
