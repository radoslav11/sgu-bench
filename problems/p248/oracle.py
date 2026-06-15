import sys

def main():
    data = sys.stdin.read().strip().split()
    # Use an iterator to consume tokens in order
    it = iter(data)

    # Read N
    n = int(next(it))
    # Read coefficients c[0..n-1]
    c = [int(next(it)) for _ in range(n)]
    # Read target value V
    V = int(next(it))

    # We solve the unbounded coin change problem:
    # dp[v] = minimum number of coins needed to form value v
    # If impossible, dp[v] will remain as a large sentinel (INF)

    INF = 10**18  # A large number to represent "infinity"
    dp = [INF] * (V + 1)

    # Base case: sum 0 can be formed with 0 coins
    dp[0] = 0

    # For each value v from 1 to V, compute dp[v]
    for v in range(1, V + 1):
        # Try using each coin denomination c[i]
        for coin in c:
            if v >= coin:
                # If dp[v - coin] is not INF, then v - coin is achievable
                prev = dp[v - coin]
                if prev != INF:
                    # One more coin than the best way to get v - coin
                    cand = prev + 1
                    if cand < dp[v]:
                        dp[v] = cand

    # After filling dp, check if V is reachable
    if dp[V] == INF:
        # No combination of c[i] sums to V
        print(-1)
    else:
        # Print minimal number of coins = minimal sum of x[i]
        print(dp[V])

if __name__ == "__main__":
    main()
