import sys


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    h = [int(x) for x in data[1:1 + n]]

    # Original perimeter and required half.
    P = 2
    for i in range(n - 1):
        P += 2 + abs(h[i] - h[i + 1])
    P += h[0] + h[-1]
    target = (P + 1) // 2

    MAXP = 5300
    NONE = -1

    # State after processing a prefix: (last kept height, kept perimeter so
    # far excluding the rightmost vertical side). last kept height 0 means no
    # board kept yet. Value = maximum stolen total height. We also store a
    # backpointer (kind, prev_last, prev_perim) for reconstruction; kind is
    # "steal" or "keep".
    dp = [[NONE] * MAXP for _ in range(101)]
    back = [[None] * MAXP for _ in range(101)]
    dp[0][0] = 0

    for pos in range(n):
        ndp = [[NONE] * MAXP for _ in range(101)]
        nback = [[None] * MAXP for _ in range(101)]
        hv = h[pos]
        for last in range(101):
            row = dp[last]
            for per in range(MAXP):
                cur = row[per]
                if cur == NONE:
                    continue
                # Steal this board: stolen sum increases, fence unchanged.
                val = cur + hv
                if val > ndp[last][per]:
                    ndp[last][per] = val
                    nback[last][per] = ("steal", last, per)
                # Keep this board: add its vertical contribution.
                # contribution = 2 + |last - hv| if a board precedes, plus the
                # left side when it is the first kept board.
                if last == 0:
                    nper = per + hv  # left side of first kept board
                else:
                    nper = per + 2 + abs(last - hv)
                if nper < MAXP and cur > ndp[hv][nper]:
                    ndp[hv][nper] = cur
                    nback[hv][nper] = ("keep", last, per)
        dp = ndp
        back = nback

    best = NONE
    best_state = None
    for last in range(101):
        for per in range(MAXP):
            if dp[last][per] == NONE:
                continue
            # Final fence perimeter = per + (right side = last) + 2 base
            # already counted? per accumulates left side + interior; add the
            # rightmost vertical side (last) and the constant 2.
            if last == 0:
                continue  # at least one board must be kept
            final_p = per + last + 2
            if final_p >= target and dp[last][per] > best:
                best = dp[last][per]
                best_state = (last, per)

    # The streaming DP above overwrote layers, so redo with full history for
    # the reconstruction.
    layers_back = []
    dp = [[NONE] * MAXP for _ in range(101)]
    dp[0][0] = 0
    for pos in range(n):
        ndp = [[NONE] * MAXP for _ in range(101)]
        nback = [[None] * MAXP for _ in range(101)]
        hv = h[pos]
        for last in range(101):
            row = dp[last]
            for per in range(MAXP):
                cur = row[per]
                if cur == NONE:
                    continue
                val = cur + hv
                if val > ndp[last][per]:
                    ndp[last][per] = val
                    nback[last][per] = ("steal", last, per)
                if last == 0:
                    nper = per + hv
                else:
                    nper = per + 2 + abs(last - hv)
                if nper < MAXP and cur > ndp[hv][nper]:
                    ndp[hv][nper] = cur
                    nback[hv][nper] = ("keep", last, per)
        layers_back.append(nback)
        dp = ndp

    last, per = best_state
    stolen = []
    for pos in range(n - 1, -1, -1):
        kind, plast, pper = layers_back[pos][last][per]
        if kind == "steal":
            stolen.append(pos + 1)
        last, per = plast, pper

    stolen.sort()
    out = [str(best), str(len(stolen)), " ".join(map(str, stolen))]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
