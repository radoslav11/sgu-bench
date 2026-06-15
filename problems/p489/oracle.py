import sys


def main():
    data = sys.stdin.read().strip().split()
    n, mod = map(int, data)

    if n == 1:
        print(1 % mod)
        return

    last_dp = [[1 % mod], [1 % mod]]

    for length in range(2, n + 1):
        pos = length - 1
        dp_up = [0] * length
        dp_down = [0] * length

        suffix = 0
        for v in range(pos - 1, -1, -1):
            suffix = (suffix + last_dp[0][v]) % mod
            dp_up[v] = suffix

        prefix = 0
        for v in range(1, length):
            prefix = (prefix + last_dp[1][v - 1]) % mod
            dp_down[v] = prefix

        last_dp = [dp_down, dp_up]

    result = sum(last_dp[0]) + sum(last_dp[1])
    print(result % mod)


if __name__ == "__main__":
    main()
