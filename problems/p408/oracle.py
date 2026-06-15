import sys

def solve():
    # Read the whole input.
    data = sys.stdin.read().strip()

    # If input is empty, do nothing.
    if not data:
        return

    # The input contains a single integer N.
    n = int(data)

    # If no points are added, the total score is zero.
    if n == 0:
        print("0.000")
        return

    # Negative infinity for impossible DP states.
    NEG_INF = -10**100

    # dp[k][d] = maximum total score after adding k points
    # and ending with maximum degree d.
    #
    # We allocate n + 1 rows for k = 0..n
    # and n + 2 columns for degrees.
    dp = [[NEG_INF] * (n + 2) for _ in range(n + 1)]

    # Base case:
    # After adding the first point:
    # - one segment exists,
    # - maximum degree is 1,
    # - diameter is 1,
    # - score is 1.
    dp[1][1] = 1.0

    # Fill DP for k = 2..n.
    for k in range(2, n + 1):

        # Maximum degree cannot exceed the number of edges k.
        for d in range(1, k + 1):

            # Maximum possible diameter for k edges and maximum degree d.
            #
            # For d >= 2:
            # diameter <= k - (d - 2)
            #
            # The expression below also handles d = 1 safely.
            dist = k - max(d - 2, 0)

            # Score gained after adding the k-th point.
            score = d * dist

            # Transition 1:
            # Previous maximum degree was also d.
            # We extend an existing arm.
            if d <= k - 1:
                dp[k][d] = max(dp[k][d], dp[k - 1][d] + score)

            # Transition 2:
            # Previous maximum degree was d - 1.
            # We add a new arm from the central point.
            if d > 1 and d - 1 <= k - 1:
                dp[k][d] = max(dp[k][d], dp[k - 1][d - 1] + score)

    # The final maximum degree can be anything from 1 to n.
    ans = max(dp[n][1:n + 1])

    # Required output accuracy is 1e-3.
    print(f"{ans:.3f}")


if __name__ == "__main__":
    solve()
