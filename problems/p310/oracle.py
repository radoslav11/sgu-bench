import sys

def main():
    # Read input
    data = sys.stdin.read().strip().split()
    n, m, k = map(int, data)

    rows = n - m + 1
    total_masks = 1 << m

    # Initialize DP table: rows x total_masks
    dp = [ [0]*total_masks for _ in range(rows) ]

    # Base case: first m bits form a mask with at least k ones
    for mask in range(total_masks):
        if mask.bit_count() >= k:
            dp[0][mask] = 1

    # Fill DP for each subsequent position
    for i in range(1, rows):
        prev = dp[i-1]
        curr = dp[i]
        for mask in range(total_masks):
            ways = prev[mask]
            if ways == 0:
                continue
            # Try appending 0 or 1
            for bit in (0, 1):
                new_mask = ((mask << 1) | bit) & (total_masks - 1)
                # Enforce at least k ones in this window
                if new_mask.bit_count() < k:
                    continue
                curr[new_mask] += ways

    # The answer is the sum of ways in the last row
    result = sum(dp[rows-1])
    print(result)

if __name__ == "__main__":
    main()
