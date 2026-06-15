import sys

MOD = 1_000_000_009


def solve():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    n = int(next(it))
    k = int(next(it))
    a = [int(next(it)) for _ in range(n)]
    b = [int(next(it)) for _ in range(n)]

    if (n - k) % 2 != 0:
        print(0)
        return

    win_b = (n - k) // 2
    win_a = win_b + k

    elements = [(x, 0) for x in a] + [(x, 1) for x in b]
    elements.sort()

    dp = [[0] * (win_b + 1) for _ in range(win_a + 1)]
    dp[0][0] = 1

    cnt_a = 0
    cnt_b = 0

    for value, team in elements:
        new_dp = [[0] * (win_b + 1) for _ in range(win_a + 1)]

        for wa in range(win_a + 1):
            row = dp[wa]
            new_row_same = new_dp[wa]
            for wb in range(win_b + 1):
                ways = row[wb]
                if ways == 0:
                    continue

                new_row_same[wb] = (new_row_same[wb] + ways) % MOD

                if team == 0:
                    candidates = cnt_b - (wa + wb)
                    if candidates > 0 and wa + 1 <= win_a:
                        new_dp[wa + 1][wb] = (new_dp[wa + 1][wb] + ways * candidates) % MOD
                else:
                    candidates = cnt_a - (wa + wb)
                    if candidates > 0 and wb + 1 <= win_b:
                        new_dp[wa][wb + 1] = (new_dp[wa][wb + 1] + ways * candidates) % MOD

        dp = new_dp

        if team == 0:
            cnt_a += 1
        else:
            cnt_b += 1

    print(dp[win_a][win_b] % MOD)


if __name__ == "__main__":
    solve()
