# Read input values:
# n, m are board sizes.
# w, h are horizontal and vertical shifts of the second board.
# k is the number of rooks to place.
n, m, w, h, k = map(int, input().split())


def overlap(lo_b, len_b, len_a):
    """
    Compute the size of the intersection of intervals:
        [0, len_a)
        [lo_b, lo_b + len_b)

    The first board always starts at coordinate 0.
    The second board starts at coordinate lo_b.
    """

    # Left border of the intersection.
    lo = max(0, lo_b)

    # Right border of the intersection.
    hi = min(len_a, lo_b + len_b)

    # If hi < lo, there is no overlap.
    return max(0, hi - lo)


# Number of shared columns between the two boards.
c_ab = overlap(w, m, n)

# Columns only in the first board.
c_a = n - c_ab

# Columns only in the second board.
c_b = m - c_ab

# Number of shared rows between the two boards.
r_both = overlap(h, m, n)

# Rows only in the first board.
r1 = n - r_both

# Rows only in the second board.
r2 = m - r_both


# dp[b][mid][t] means:
# b   = number of unused first-only rows,
# mid = number of unused shared rows,
# t   = number of unused second-only rows.
dp = [
    [
        [0 for _ in range(r2 + 1)]
        for _ in range(r_both + 1)
    ]
    for _ in range(r1 + 1)
]

# Initially, every row is unused.
dp[r1][r_both][r2] = 1


def process_column(use_r1, use_both, use_r2):
    """
    Process one concrete column.

    use_r1:
        whether this column can place a rook in a first-only row.

    use_both:
        whether this column can place a rook in a shared row.

    use_r2:
        whether this column can place a rook in a second-only row.
    """

    global dp

    # New DP table after processing this column.
    ndp = [
        [
            [0 for _ in range(r2 + 1)]
            for _ in range(r_both + 1)
        ]
        for _ in range(r1 + 1)
    ]

    # Iterate over all possible states.
    for b in range(r1 + 1):
        for mid in range(r_both + 1):
            for t in range(r2 + 1):

                # Current number of ways to reach this state.
                cur = dp[b][mid][t]

                # Skip unreachable states.
                if cur == 0:
                    continue

                # Option 1: place no rook in this column.
                ndp[b][mid][t] += cur

                # Option 2: place a rook in a first-only row.
                if use_r1 and b > 0:
                    # There are b choices for the actual row.
                    ndp[b - 1][mid][t] += cur * b

                # Option 3: place a rook in a shared row.
                if use_both and mid > 0:
                    # There are mid choices for the actual row.
                    ndp[b][mid - 1][t] += cur * mid

                # Option 4: place a rook in a second-only row.
                if use_r2 and t > 0:
                    # There are t choices for the actual row.
                    ndp[b][mid][t - 1] += cur * t

    # Replace the old table with the updated one.
    dp = ndp


# First-only columns can use first-only rows and shared rows.
for _ in range(c_a):
    process_column(True, True, False)

# Shared columns can use all row types.
for _ in range(c_ab):
    process_column(True, True, True)

# Second-only columns can use shared rows and second-only rows.
for _ in range(c_b):
    process_column(False, True, True)


# Accumulate the answer from states where exactly k rows were consumed.
answer = 0

for b in range(r1 + 1):
    for mid in range(r_both + 1):
        for t in range(r2 + 1):

            # Used rows correspond exactly to placed rooks.
            rooks = (r1 - b) + (r_both - mid) + (r2 - t)

            # Keep only placements with exactly k rooks.
            if rooks == k:
                answer += dp[b][mid][t]

# Print the exact result.
print(answer)
