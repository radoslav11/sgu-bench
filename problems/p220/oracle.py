import sys

sys.setrecursionlimit(10000)


def read_input():
    return map(int, sys.stdin.read().split())


def diag_lengths(n, color):
    """diag_len[d] = number of squares of given color on diagonal r+c = d."""
    lens = [0] * (2 * n - 1)
    for r in range(n):
        for c in range(n):
            if (r + c) % 2 == color:
                d = r + c
                lens[d] += 1
    return lens


def ways_on_color(n, k, color):
    """ways[b] = number of ways to place b mutually non-attacking bishops
    on all squares of the given color, DP over diagonals sorted by length."""
    lens = sorted(diag_lengths(n, color))
    dp = [0] * (k + 1)
    dp[0] = 1
    for cnt in lens:
        new_dp = dp[:]
        for placed in range(1, k + 1):
            free_spots = cnt - (placed - 1)
            if free_spots > 0:
                new_dp[placed] += dp[placed - 1] * free_spots
        dp = new_dp
    return dp


def main():
    data = list(read_input())
    n, k = data[0], data[1]
    ways_black = ways_on_color(n, k, 0)
    ways_white = ways_on_color(n, k, 1)
    ans = sum(ways_black[i] * ways_white[k - i] for i in range(k + 1))
    print(ans)


if __name__ == "__main__":
    main()
