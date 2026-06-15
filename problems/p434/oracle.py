import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    S = [int(next(it)) for _ in range(n)]
    D = [int(next(it)) for _ in range(n)]

    # Feasibility: total liquid must match
    if sum(S) != sum(D):
        print(-1)
        return

    # diff[i] is how much tube i must send out (positive) or receive (negative)
    diff = [S[i] - D[i] for i in range(n)]

    size = 1 << n

    # Precompute sum[mask] = sum(diff[i] for i in mask)
    # Use lowbit technique to do it in O(2^n)
    ssum = [0] * size
    for mask in range(1, size):
        lb = mask & -mask                 # lowest set bit (power of two)
        bit = (lb.bit_length() - 1)       # index of that bit
        ssum[mask] = ssum[mask ^ lb] + diff[bit]

    # dp[mask] = maximum number of balanced (sum==0) groups obtainable from mask
    dp = [0] * size
    for mask in range(1, size):
        best = 0

        # Try removing each present element and inherit the best dp
        # This is O(n) per mask, total O(n * 2^n)
        m = mask
        while m:
            lb = m & -m
            best = max(best, dp[mask ^ lb])
            m ^= lb

        # If this subset itself is balanced, we can form one more group
        if ssum[mask] == 0:
            best += 1

        dp[mask] = best

    gmax = dp[size - 1]
    # Minimum pourings = N - max_number_of_zero_sum_groups
    print(n - gmax)

if __name__ == "__main__":
    solve()
