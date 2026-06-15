import sys

sys.setrecursionlimit(100000)


def main():
    n, m = map(int, sys.stdin.readline().split())
    a = [list(map(int, sys.stdin.readline().split())) for _ in range(n)]

    dp = [[None] * (m + 1) for _ in range(n + 1)]
    best_dec = [[False] * (m + 1) for _ in range(n + 1)]

    NEG_INF = -10**9

    def rec(i, j):
        if i == n:
            return 0
        if j == m:
            return NEG_INF
        if dp[i][j] is not None:
            return dp[i][j]

        skip = rec(i, j + 1)
        take = a[i][j] + rec(i + 1, j + 1)

        if take > skip:
            dp[i][j] = take
            best_dec[i][j] = True
        else:
            dp[i][j] = skip
            best_dec[i][j] = False
        return dp[i][j]

    max_score = rec(0, 0)
    print(max_score)

    res = []
    i = j = 0
    while i < n and j < m:
        if best_dec[i][j]:
            res.append(j + 1)
            i += 1
            j += 1
        else:
            j += 1

    print(" ".join(map(str, res)))


if __name__ == "__main__":
    main()
