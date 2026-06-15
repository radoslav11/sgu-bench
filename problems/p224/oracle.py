import sys
from functools import lru_cache


def count_configurations(n, k):
    # Bitmasks per row: attacked columns, main diagonals (shift left
    # when advancing a row) and anti diagonals (shift right).
    ALL = (1 << n) - 1

    @lru_cache(None)
    def dfs(row, placed, cols, d1, d2):
        if placed == k:
            return 1
        if row == n:
            return 0
        if placed + (n - row) < k:
            return 0

        ways = dfs(row + 1, placed, cols, (d1 << 1) & ALL, d2 >> 1)

        attacked = cols | d1 | d2
        free_positions = (~attacked) & ALL
        while free_positions:
            bit = free_positions & -free_positions
            free_positions -= bit
            ways += dfs(
                row + 1,
                placed + 1,
                cols | bit,
                ((d1 | bit) << 1) & ALL,
                (d2 | bit) >> 1,
            )
        return ways

    return dfs(0, 0, 0, 0, 0)


if __name__ == "__main__":
    n, k = map(int, sys.stdin.read().split())
    print(count_configurations(n, k))
