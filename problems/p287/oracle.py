import sys
# Import sys for fast input reading.


def solve():
    # Read all input tokens.
    data = sys.stdin.read().split()

    # q is the number of possible hidden values.
    q = int(data[0])

    # c is the response delay.
    c = int(data[1])

    # We define:
    #
    # dp[n] = maximum interval size solvable in n guesses.
    #
    # Base:
    #   dp[n] = 0 for n <= c
    #
    # Recurrence:
    #   dp[n] = dp[n - 1] + dp[n - c - 1] + 1
    #
    # We only need two older values:
    #   dp[n - 1]
    #   dp[n - c - 1]
    #
    # Therefore a circular buffer of size c + 2 is enough.
    size = c + 2

    # Initialize circular buffer with zeros.
    # These zeros also represent all base values dp[n] = 0 for n <= c.
    dp = [0] * size

    # Start checking n = 1, 2, 3, ...
    n = 1

    while True:
        # By default, dp[n] is 0.
        # This is correct when n <= c because no answer has arrived yet.
        value = 0

        if n > c:
            # Apply recurrence:
            #
            # dp[n] = dp[n - 1] + dp[n - c - 1] + 1
            #
            # Use modulo indexing because dp is stored as a circular buffer.
            value = dp[(n - 1) % size] + dp[(n - c - 1) % size] + 1

        # Store dp[n] in the circular buffer.
        dp[n % size] = value

        # Once we can cover at least q numbers, n guesses are sufficient.
        # Because n increases one by one, this n is minimal.
        if value >= q:
            print(n)
            return

        # Try the next number of guesses.
        n += 1


if __name__ == "__main__":
    # Run the solver.
    solve()
