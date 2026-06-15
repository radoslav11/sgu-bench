import sys
from array import array

# Large value representing an impossible state.
INF = 10 ** 9


def solve():
    # Read Jack's notes.
    s = sys.stdin.readline().strip()

    # This will store the reduced form after deleting forced observed matches.
    reduced = []

    # Number of observed non-star letters.
    # These actions always count in the final total.
    kept = 0

    # Greedy reduction.
    for ch in s:
        # Count every observed action.
        if ch != '*':
            kept += 1

        # A star or lowercase push is appended directly.
        if ch == '*' or ('a' <= ch <= 'z'):
            reduced.append(ch)

        else:
            # ch is uppercase, meaning an observed pop.

            # If there is no lowercase push immediately available to match,
            # keep this uppercase as unresolved.
            if not reduced or reduced[-1] == '*' or ('A' <= reduced[-1] <= 'Z'):
                reduced.append(ch)

            # If the previous unresolved symbol is the matching lowercase,
            # this observed pair is forced and can be removed.
            elif reduced[-1] == ch.lower():
                reduced.pop()

            # Otherwise the top dish would have the wrong color.
            else:
                print(-1)
                return

    # Reduced string.
    t = ''.join(reduced)

    # Length of reduced string.
    n = len(t)

    # If everything was reduced away, no missing action is needed.
    if n == 0:
        print(kept // 2)
        return

    # Precompute character types for speed and clarity.
    is_low = [('a' <= c <= 'z') for c in t]
    is_up = [('A' <= c <= 'Z') for c in t]

    # lowercase version of each character, used for comparing colors.
    lower = [c.lower() for c in t]

    # Collect useful split points.
    split_pts = []

    for i in range(n):
        # Split at a star.
        if t[i] == '*':
            split_pts.append(i)

        # Split between an uppercase block and lowercase block.
        elif i + 1 < n and is_up[i] and is_low[i + 1]:
            split_pts.append(i)

    # dp[l][r] = minimum number of inserted actions needed
    # to make t[l..r] balanced.
    #
    # Using array('i') keeps memory much smaller than a list of Python ints.
    dp = [array('i', [-1]) * n for _ in range(n)]

    # Fill intervals by increasing length.
    # All transitions go to shorter intervals, so this order works.
    for length in range(1, n + 1):
        for l in range(0, n - length + 1):
            r = l + length - 1

            # Start as impossible.
            best = INF

            # Case 1: both endpoints are stars.
            if t[l] == '*' and t[r] == '*':
                # Ignore/drop left star for this segment.
                val = 0 if l + 1 > r else dp[l + 1][r]
                if val < best:
                    best = val

                # Ignore/drop right star for this segment.
                val = 0 if l > r - 1 else dp[l][r - 1]
                if val < best:
                    best = val

            # Case 2: left endpoint is a star.
            elif t[l] == '*':
                # If right endpoint is uppercase, left star can generate
                # its matching lowercase push.
                if is_up[r]:
                    val = 0 if l > r - 1 else dp[l][r - 1]
                    if val + 1 < best:
                        best = val + 1

                # Or left star contributes nothing useful here.
                val = 0 if l + 1 > r else dp[l + 1][r]
                if val < best:
                    best = val

            # Case 3: right endpoint is a star.
            elif t[r] == '*':
                # If left endpoint is lowercase, right star can generate
                # its matching uppercase pop.
                if is_low[l]:
                    val = 0 if l + 1 > r else dp[l + 1][r]
                    if val + 1 < best:
                        best = val + 1

                # Or right star contributes nothing useful here.
                val = 0 if l > r - 1 else dp[l][r - 1]
                if val < best:
                    best = val

            # Case 4: both endpoints are observed letters.
            # They may match if left is lowercase, right is uppercase,
            # and both have the same color.
            elif is_low[l] and is_up[r] and lower[l] == lower[r]:
                val = 0 if l + 1 > r - 1 else dp[l + 1][r - 1]
                if val < best:
                    best = val

            # Try all useful split points.
            for i in split_pts:
                # Star split:
                # The star can be shared by both sides,
                # so split as [l..i] and [i..r].
                if t[i] == '*':
                    if l < i < r:
                        val = dp[l][i] + dp[i][r]
                        if val < best:
                            best = val

                # Ordinary split after position i:
                # split as [l..i] and [i+1..r].
                else:
                    if l <= i < r:
                        val = dp[l][i] + dp[i + 1][r]
                        if val < best:
                            best = val

            # Store answer for interval [l, r].
            dp[l][r] = best

    # Minimum missing actions for the whole reduced string.
    extra = dp[0][n - 1]

    # If impossible, report -1.
    if extra >= INF:
        print(-1)
    else:
        # Total actions = observed actions + inserted actions.
        # Each dish has exactly one push and one pop.
        print((kept + extra) // 2)


if __name__ == "__main__":
    solve()
