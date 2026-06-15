import sys

import numpy as np


def xor_fwt(a):
    """In-place Walsh-Hadamard transform over a numpy int64 vector."""
    n = len(a)
    h = 1
    while h < n:
        for i in range(0, n, h * 2):
            x = a[i:i + h].copy()
            y = a[i + h:i + 2 * h].copy()
            a[i:i + h] = x + y
            a[i + h:i + 2 * h] = x - y
        h <<= 1


def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it)) - 1
    R = [[int(next(it)) for _ in range(n)] for __ in range(n)]

    # best[k] = minimal tree height for k leaves; STEPS-1 = height for n
    best = [0] * (n + 1)
    for i in range(1, n + 1):
        best[i] = best[i >> 1] + 1
    STEPS = best[n - 1] + 1

    FULL = 1 << n

    # dp[step][winner][size] = FWT-domain count vector (or None)
    dp = [[[None] * (n + 1) for __ in range(n)] for ___ in range(STEPS)]

    for i in range(n):
        vec = np.zeros(FULL, dtype=np.int64)
        vec[1 << i] = 1
        xor_fwt(vec)
        dp[0][i][1] = vec

    splits = [(a, b) for a in range(1, n + 1) for b in range(1, n + 1)
              if a + b <= n]
    splits.sort(key=lambda x: max(x))

    for sa, sb in splits:
        for s1 in range(STEPS - 1):
            if s1 < best[sa - 1]:
                continue
            for s2 in range(STEPS - 1):
                if s2 < best[sb - 1]:
                    continue
                s_new = max(s1, s2) + 1
                for x in range(n):
                    VA = dp[s1][x][sa]
                    if VA is None:
                        continue
                    for y in range(x + 1, n):
                        VB = dp[s2][y][sb]
                        if VB is None:
                            continue
                        w = x if R[x][y] else y
                        left = n - sa - sb
                        if left != 0 and STEPS - s_new - 1 == 0:
                            continue
                        if dp[s_new][w][sa + sb] is None:
                            dp[s_new][w][sa + sb] = np.zeros(
                                FULL, dtype=np.int64)
                        dp[s_new][w][sa + sb] += VA * VB

    res_vec = dp[STEPS - 1][m][n]
    if res_vec is None:
        print(0)
        return
    xor_fwt(res_vec)
    print(int(res_vec[FULL - 1]) // FULL)


if __name__ == "__main__":
    main()
