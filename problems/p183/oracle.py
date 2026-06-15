import sys
input = sys.stdin.readline


def main():
    INF = 10**18
    n, m = map(int, input().split())
    a = list(map(int, input().split()))

    # dp[i][d]: minimal cost if ball i is painted black,
    # and distance to the previous painted ball is exactly d (1..m)
    dp = [[INF] * (m + 1) for _ in range(n)]

    for i in range(n):
        d = i + 1
        if d < m:
            dp[i][d] = a[i]
        else:
            break

    answer = INF

    for i in range(n):
        prefix_min = [INF] * (m + 1)
        running = INF
        for d in range(1, m + 1):
            if dp[i][d] < running:
                running = dp[i][d]
            prefix_min[d] = running

        tail_gap = n - i
        for d in range(1, m + 1):
            cost_here = dp[i][d]
            if cost_here >= INF:
                continue
            if d + tail_gap <= m:
                answer = min(answer, cost_here)

        for dist in range(1, m + 1):
            j = i + dist
            if j >= n:
                break
            lim = m - dist
            if lim <= 0:
                continue
            best_prev = prefix_min[lim]
            new_cost = best_prev + a[j]
            if new_cost < dp[j][dist]:
                dp[j][dist] = new_cost

    print(answer)


if __name__ == "__main__":
    main()
