import sys


def solve():
    # Read the regular expression.
    regex = sys.stdin.readline().strip()

    # Expand '!' into '???', because '!' means exactly three arbitrary letters.
    s = []
    for ch in regex:
        if ch == "!":
            s.append("???")
        else:
            s.append(ch)

    # Join pieces into the expanded expression.
    s = "".join(s)

    # Length after expansion.
    n = len(s)

    # Empty expression can produce the empty palindrome.
    if n == 0:
        print("YES")
        print()
        return

    # Large value representing impossible states.
    INF = 10**9

    # dp[l][r] = minimum length of palindrome obtainable from s[l:r+1].
    dp = [[INF] * n for _ in range(n)]

    # Helper function: is s[i] a star?
    def is_star(i):
        return s[i] == "*"

    # Helper function: is s[i] a question mark?
    def is_q(i):
        return s[i] == "?"

    # Helper function for DP intervals.
    # Empty interval has answer 0.
    def get_dp(l, r):
        if l > r:
            return 0
        return dp[l][r]

    # Initialize intervals of length 1.
    for i in range(n):
        if is_star(i):
            # '*' may become empty.
            dp[i][i] = 0
        else:
            # A letter or '?' must become exactly one character.
            dp[i][i] = 1

    # Fill DP by increasing interval length.
    for length in range(2, n + 1):
        for l in range(0, n - length + 1):
            r = l + length - 1
            best = INF

            # Left '*' becomes empty.
            if is_star(l) and get_dp(l + 1, r) < INF:
                best = min(best, get_dp(l + 1, r))

            # Right '*' becomes empty.
            if is_star(r) and get_dp(l, r - 1) < INF:
                best = min(best, get_dp(l, r - 1))

            # Both ends are not '*'.
            if not is_star(l) and not is_star(r):
                inner = get_dp(l + 1, r - 1)

                # Boundary characters are compatible unless both are fixed and different.
                ok = True
                if not is_q(l) and not is_q(r) and s[l] != s[r]:
                    ok = False

                if ok and inner < INF:
                    best = min(best, inner + 2)

            # Left is not '*', right is '*'.
            elif not is_star(l) and is_star(r):
                inner = get_dp(l + 1, r)

                # Right '*' generates a matching outer character.
                if inner < INF:
                    best = min(best, inner + 2)

            # Left is '*', right is not '*'.
            elif is_star(l) and not is_star(r):
                inner = get_dp(l, r - 1)

                # Left '*' generates a matching outer character.
                if inner < INF:
                    best = min(best, inner + 2)

            # Store the best result for this interval.
            dp[l][r] = best

    # If full expression cannot produce a palindrome.
    if dp[0][n - 1] >= INF:
        print("NO")
        return

    # Rolling layers of reconstructed strings.
    # lm2 stores interval length length - 2.
    # lm1 stores interval length length - 1.
    lm2 = [""] * n
    lm1 = [""] * n

    answer = ""

    # Reconstruct lexicographically smallest optimal palindrome.
    for length in range(1, n + 1):
        # Current layer for this interval length.
        cur = [""] * n

        for l in range(0, n - length + 1):
            r = l + length - 1

            # Impossible interval.
            if dp[l][r] >= INF:
                continue

            # Base case: one symbol.
            if length == 1:
                if is_star(l):
                    # '*' can produce empty.
                    cur[l] = ""
                elif is_q(l):
                    # '?' should become smallest possible letter.
                    cur[l] = "a"
                else:
                    # Fixed letter.
                    cur[l] = s[l]
                continue

            # Required optimal length.
            target = dp[l][r]

            # Best candidate string found so far.
            best = None

            # Helper to update best candidate.
            def try_candidate(cand):
                nonlocal best
                if best is None or cand < best:
                    best = cand

            # Case 1: both ends are not '*'.
            if not is_star(l) and not is_star(r):
                inner_len = 0 if length == 2 else dp[l + 1][r - 1]

                # This transition must achieve the optimal length.
                if inner_len != INF and inner_len + 2 == target:
                    ok = True
                    c = ""

                    if not is_q(l) and not is_q(r):
                        # Both are fixed letters.
                        if s[l] == s[r]:
                            c = s[l]
                        else:
                            ok = False
                    elif not is_q(l):
                        # Left fixed, right '?'.
                        c = s[l]
                    elif not is_q(r):
                        # Right fixed, left '?'.
                        c = s[r]
                    else:
                        # Both '?'.
                        c = "a"

                    if ok:
                        # Inner palindrome.
                        inner_pal = "" if length == 2 else lm2[l + 1]

                        # Build candidate.
                        cand = c + inner_pal + c

                        # Consider candidate.
                        try_candidate(cand)

            # Case 2: left is not '*', right is '*'.
            if not is_star(l) and is_star(r):
                inner_len = dp[l + 1][r]

                # Right '*' generates the matching outer character.
                if inner_len != INF and inner_len + 2 == target:
                    c = "a" if is_q(l) else s[l]
                    inner_pal = lm1[l + 1]
                    cand = c + inner_pal + c
                    try_candidate(cand)

            # Case 3: left is '*', right is not '*'.
            if is_star(l) and not is_star(r):
                inner_len = dp[l][r - 1]

                # Left '*' generates the matching outer character.
                if inner_len != INF and inner_len + 2 == target:
                    c = "a" if is_q(r) else s[r]
                    inner_pal = lm1[l]
                    cand = c + inner_pal + c
                    try_candidate(cand)

            # Case 4: left '*' becomes empty.
            if is_star(l) and dp[l + 1][r] == target:
                try_candidate(lm1[l + 1])

            # Case 5: right '*' becomes empty.
            if is_star(r) and dp[l][r - 1] == target:
                try_candidate(lm1[l])

            # Store best candidate for current interval.
            cur[l] = best if best is not None else ""

        # Full interval reconstructed.
        if length == n:
            answer = cur[0]
            break

        # Rotate layers.
        lm2, lm1 = lm1, cur

    # Output result.
    print("YES")
    print(answer)


if __name__ == "__main__":
    solve()
