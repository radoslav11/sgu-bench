import sys
from functools import lru_cache


def count_tilings(M, N):
    # Row-by-row DP. State between rows: bitmask of cells in the next row
    # already covered by pieces sticking out of the current row.
    W = N

    def row_transitions(last_row):
        # Returns dict: in_mask -> {out_mask: ways}
        result = {}
        for in_mask in range(1 << W):
            ways = {}

            def dfs(j, cur, nxt):
                if j == W:
                    ways[nxt] = ways.get(nxt, 0) + 1
                    return
                if cur & (1 << j):
                    dfs(j + 1, cur, nxt)
                    return
                # Horizontal domino (j, j+1) in current row.
                if j + 1 < W and not (cur & (1 << (j + 1))):
                    dfs(j + 2, cur | (1 << (j + 1)), nxt)
                if not last_row:
                    nj = not (nxt & (1 << j))
                    nj1 = j + 1 < W and not (nxt & (1 << (j + 1)))
                    njm1 = j - 1 >= 0 and not (nxt & (1 << (j - 1)))
                    cj1 = j + 1 < W and not (cur & (1 << (j + 1)))
                    # Vertical domino.
                    if nj:
                        dfs(j + 1, cur, nxt | (1 << j))
                    # L missing bottom-right: cur j, j+1 + next j.
                    if cj1 and nj:
                        dfs(j + 2, cur | (1 << (j + 1)), nxt | (1 << j))
                    # L missing bottom-left: cur j, j+1 + next j+1.
                    if cj1 and nj1:
                        dfs(j + 2, cur | (1 << (j + 1)), nxt | (1 << (j + 1)))
                    # L missing top-right: cur j + next j, j+1.
                    if nj and nj1:
                        dfs(j + 1, cur, nxt | (1 << j) | (1 << (j + 1)))
                    # L missing top-left: cur j + next j-1, j.
                    if njm1 and nj:
                        dfs(j + 1, cur, nxt | (1 << (j - 1)) | (1 << j))

            dfs(0, in_mask, 0)
            result[in_mask] = ways
        return result

    mid = row_transitions(False)
    last = row_transitions(True)

    dp = {0: 1}
    for r in range(M):
        trans = last if r == M - 1 else mid
        ndp = {}
        for in_mask, cnt in dp.items():
            for out_mask, w in trans[in_mask].items():
                ndp[out_mask] = ndp.get(out_mask, 0) + cnt * w
        dp = ndp
    return dp.get(0, 0)


if __name__ == "__main__":
    M, N = map(int, sys.stdin.read().split())
    print(count_tilings(M, N))
