import sys
import math


def solve():
    # Read all input tokens.
    data = sys.stdin.read().strip().split()

    # If there is no input, do nothing.
    if not data:
        return

    # Parse input.
    n = int(data[0])
    f1 = float(data[1])
    f2 = float(data[2])
    profit = float(data[3])

    # val1[a][b] = equilibrium expected net payoff of player 1 from state (a,b).
    val1 = [[0.0 for _ in range(n + 1)] for _ in range(n + 1)]

    # val2[a][b] = equilibrium expected net payoff of player 2 from state (a,b).
    val2 = [[0.0 for _ in range(n + 1)] for _ in range(n + 1)]

    # win1[a][b] = probability that player 1 eventually wins from state (a,b).
    win1 = [[0.0 for _ in range(n + 1)] for _ in range(n + 1)]

    # Logarithms used in the inverse bribe formula.
    # Since 0 < f < 1, these are negative.
    log1 = math.log(1.0 - f1)
    log2 = math.log(1.0 - f2)

    # Minimum possible daily approval probability, achieved with zero bribe.
    LO = 0.01

    # Probability 1 requires infinite bribe, so use something just below 1.
    HI = 1.0 - 1e-12

    def resolve(board, x, y, won, lost, tie):
        """
        Return value for state (x,y).

        If the state is terminal, return the corresponding terminal value.
        Otherwise, return board[x][y], which has already been computed.
        """
        if x == n and y == n:
            return tie
        if x == n:
            return won
        if y == n:
            return lost
        return board[x][y]

    def bribe(p, log_base):
        """
        Convert success probability p into required bribe.

        p = 1 - 0.99 * (1-F)^b

        Therefore:

        b = log((1-p)/0.99) / log(1-F)
        """
        return math.log((1.0 - p) / 0.99) / log_base

    def expected(p1, p2, both, only1, only2):
        """
        Expected future value excluding the self-loop where both fail.

        both: value if both succeed
        only1: value if only player 1 succeeds
        only2: value if only player 2 succeeds
        """
        return (
            p1 * p2 * both
            + p1 * (1.0 - p2) * only1
            + (1.0 - p1) * p2 * only2
        )

    def argmax(func):
        """
        Maximize a single-peaked function on [LO, HI] using ternary search.
        """
        left = LO
        right = HI

        # Fixed iteration count is enough for double precision.
        for _ in range(80):
            m1 = left + (right - left) / 3.0
            m2 = right - (right - left) / 3.0

            if func(m1) < func(m2):
                # Maximum is to the right.
                left = m1
            else:
                # Maximum is to the left.
                right = m2

        return (left + right) / 2.0

    # Process states in reverse order.
    # Any transition increases at least one coordinate, so future states are known.
    for a in range(n - 1, -1, -1):
        for b in range(n - 1, -1, -1):
            # Player 1's values after the possible successful transitions.
            a1 = resolve(val1, a + 1, b + 1, profit, 0.0, profit / 2.0)
            b1 = resolve(val1, a + 1, b, profit, 0.0, profit / 2.0)
            c1 = resolve(val1, a, b + 1, profit, 0.0, profit / 2.0)

            # Player 2's values after the possible successful transitions.
            # From player 2's perspective, if player 1 reaches n first, player 2 loses.
            a2 = resolve(val2, a + 1, b + 1, 0.0, profit, profit / 2.0)
            b2 = resolve(val2, a + 1, b, 0.0, profit, profit / 2.0)
            c2 = resolve(val2, a, b + 1, 0.0, profit, profit / 2.0)

            def v1(p1, p2):
                """
                Expected net payoff of player 1 for fixed probabilities p1,p2.
                """
                leave = p1 + p2 - p1 * p2
                daily_cost = bribe(p1, log1)
                future = expected(p1, p2, a1, b1, c1)
                return (-daily_cost + future) / leave

            def v2(p1, p2):
                """
                Expected net payoff of player 2 for fixed probabilities p1,p2.
                """
                leave = p1 + p2 - p1 * p2
                daily_cost = bribe(p2, log2)
                future = expected(p1, p2, a2, b2, c2)
                return (-daily_cost + future) / leave

            def br1(p2):
                """
                Player 1's best response to player 2 choosing p2.
                """
                return argmax(lambda q: v1(q, p2))

            def br2(p1):
                """
                Player 2's best response to player 1 choosing p1.
                """
                return argmax(lambda q: v2(p1, q))

            def gap(p1):
                """
                Fixed-point residual.

                At equilibrium:
                    p1 = br1(br2(p1))
                """
                return br1(br2(p1)) - p1

            # Find equilibrium p1 by bisection.
            left = LO
            right = HI

            for _ in range(100):
                mid = (left + right) / 2.0

                if gap(mid) > 0.0:
                    left = mid
                else:
                    right = mid

            # Equilibrium probabilities.
            p1 = (left + right) / 2.0
            p2 = br2(p1)

            # Store expected net payoffs at equilibrium.
            val1[a][b] = v1(p1, p2)
            val2[a][b] = v2(p1, p2)

            # Values of player 1's winning probability after transitions.
            wa = resolve(win1, a + 1, b + 1, 1.0, 0.0, 0.5)
            wb = resolve(win1, a + 1, b, 1.0, 0.0, 0.5)
            wc = resolve(win1, a, b + 1, 1.0, 0.0, 0.5)

            # Probability that at least one player succeeds and the state changes.
            leave = p1 + p2 - p1 * p2

            # Compute player 1's winning probability, removing the self-loop.
            win1[a][b] = expected(p1, p2, wa, wb, wc) / leave

    # Player 2's winning probability is the complement.
    ans1 = win1[0][0]
    ans2 = 1.0 - ans1

    # Print with sufficient precision.
    print(f"{ans1:.9f} {ans2:.9f}")


if __name__ == "__main__":
    solve()
