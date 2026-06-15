import sys

def solve() -> None:
    # Read A as Python int (supports arbitrarily large integers)
    A_line = sys.stdin.readline().strip()
    B_line = sys.stdin.readline().strip()

    A = int(A_line)
    B = int(B_line)

    # Special case: B = 1
    # All powers are 1, so we need exactly A copies of +1 to sum to A.
    if B == 1:
        print(A)
        return

    # Convert A to base-B digits, least significant first.
    digits = []
    x = A
    while x > 0:
        digits.append(x % B)
        x //= B
    L = len(digits)

    # dp_next[c] will store dp[i+1][c] while iterating i downward.
    # Base case at i = L:
    # dp[L][0] = 0, dp[L][1] = 1 (leftover carry becomes one extra term)
    dp_next0, dp_next1 = 0, 1

    # Process digits from most significant to least (i = L-1 ... 0)
    for i in range(L - 1, -1, -1):
        d = digits[i]

        # Compute dp[i][0] and dp[i][1]

        # For carry c=0:
        t = d
        r = t % B
        q = t // B  # always 0 here because 0 <= d < B
        best0 = r + (dp_next0 if q == 0 else dp_next1)
        if r != 0:
            # borrow: digit becomes r-B, next carry q+1 = 1
            best0 = min(best0, (B - r) + dp_next1)

        # For carry c=1:
        t = d + 1
        r = t % B
        q = t // B  # either 0 or 1
        best1 = r + (dp_next0 if q == 0 else dp_next1)
        if r != 0:
            # This is only possible when q == 0, hence q+1 == 1 is valid
            best1 = min(best1, (B - r) + dp_next1)

        # Shift window: dp_next becomes dp[i]
        dp_next0, dp_next1 = best0, best1

    # Answer is dp[0][0]
    print(dp_next0)

if __name__ == "__main__":
    solve()
