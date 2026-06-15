import sys
def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    # dp[l][M][k] = probability of a derangement of size l
    # with no cycle > M, and exactly k cycles of size M
    dp = [ [ [] for _ in range(n+1) ] for __ in range(n+1) ]
    # Pre-allocate inner lists by max possible k = floor(l/M)
    for l in range(n+1):
        for M in range(1, n+1):
            maxCnt = l // M
            dp[l][M] = [0.0] * (maxCnt+1)

    # Base case: empty set has 1 way, zero cycles
    for M in range(1, n+1):
        dp[0][M][0] = 1.0

    # Build DP for lengths 1..n
    for l in range(1, n+1):
        for M in range(1, n+1):
            maxCnt = l // M
            for k in range(maxCnt+1):
                # Choose the cycle size s containing '1'
                # must be in [2..min(M,l)]
                for s in range(2, min(M, l)+1):
                    prev_k = k - (1 if s == M else 0)
                    if prev_k < 0 or prev_k >= len(dp[l-s][M]):
                        continue
                    # We add probability mass 1/l
                    dp[l][M][k] += dp[l-s][M][prev_k] / l

    # Sum up total mass of valid derangements Z
    Z = 0.0
    for M in range(2, n+1):
        for k in range(1, n//M + 1):
            Z += dp[n][M][k]

    # Compute expected total size of largest cycles: E[k * M]
    ans = 0.0
    for M in range(2, n+1):
        for k in range(1, n//M + 1):
            prob = dp[n][M][k] / Z
            ans += prob * (k * M)

    # Print with 9 decimal places
    print(f"{ans:.9f}")

if __name__ == "__main__":
    main()
