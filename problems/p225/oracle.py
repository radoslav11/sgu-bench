import sys

import numpy as np


def solve():
    n, k = map(int, sys.stdin.read().split())

    # Max non-attacking knights: all n^2 cells for n <= 2 (no knight move
    # fits), otherwise one color class, ceil(n^2 / 2).
    cap = n * n if n <= 2 else (n * n + 1) // 2
    if k > cap:
        print(0)
        return

    # Cell-by-cell DP over a sliding window of the last W = 2n+1 cells.
    # Bit (d-1) of the window mask is the occupancy of the cell d
    # positions back in row-major order. A knight at (r, c) attacks the
    # earlier cells (r-1, c+-2) and (r-2, c+-1), i.e. back-distances
    # n-2, n+2, 2n-1, 2n+1. dp[j][mask] = ways with j knights placed.
    W = 2 * n + 1
    M = 1 << W
    half = M >> 1

    dp = np.zeros((k + 1, M), dtype=np.int64)
    dp[0, 0] = 1
    ndp = np.zeros((k + 1, M), dtype=np.int64)

    masks = np.arange(half, dtype=np.int64)

    for pos in range(n * n):
        r, c = divmod(pos, n)
        fm = 0
        if r >= 1 and c + 2 < n:
            fm |= 1 << (n - 3)
        if r >= 1 and c >= 2:
            fm |= 1 << (n + 1)
        if r >= 2 and c + 1 < n:
            fm |= 1 << (2 * n - 2)
        if r >= 2 and c >= 1:
            fm |= 1 << (2 * n)

        okl = ((masks & fm) == 0)
        okh = (((masks | half) & fm) == 0)

        # At most pos+1 knights fit in the first pos+1 cells; dp rows
        # above the previous cap are stale buffer contents, never read.
        kk_read = min(k, pos)
        kk = min(k, pos + 1)
        for j in range(kk, -1, -1):
            # Skip the cell: new mask = (2 * m) mod M.
            if j <= kk_read:
                np.add(dp[j, :half], dp[j, half:], out=ndp[j, 0::2])
            else:
                ndp[j, 0::2] = 0
            # Place a knight: new mask = (2 * m + 1) mod M.
            if j >= 1:
                np.multiply(dp[j - 1, :half], okl, out=ndp[j, 1::2])
                ndp[j, 1::2] += dp[j - 1, half:] * okh
            else:
                ndp[j, 1::2] = 0
        dp, ndp = ndp, dp

    print(int(dp[k].sum()))


if __name__ == "__main__":
    solve()
