import sys

def solve() -> None:
    data = list(map(int, sys.stdin.read().strip().split()))
    n = data[0]
    h = data[1:1+n]

    # Two pointers at the ends (0-based indices)
    l, r = 0, n - 1

    # Feasible synchronized height interval [low, high]
    # Start at height 0 on both ends (given by statement)
    low = high = 0

    while l < r:
        # Case 1: safe advance from left if next height is within [low, high]
        if low <= h[l + 1] <= high:
            l += 1

        # Case 1 (symmetric): safe advance from right
        elif low <= h[r - 1] <= high:
            r -= 1

        # Case 2: both next heights above current high -> must raise high
        elif h[l + 1] > high and h[r - 1] > high:
            # Raise to the smaller one and move that pointer inward
            if h[l + 1] < h[r - 1]:
                high = h[l + 1]
                l += 1
            else:
                high = h[r - 1]
                r -= 1

        # Case 3: both next heights below current low -> must lower low
        elif h[l + 1] < low and h[r - 1] < low:
            # Lower to the larger one and move that pointer inward
            if h[l + 1] > h[r - 1]:
                low = h[l + 1]
                l += 1
            else:
                low = h[r - 1]
                r -= 1

        # Case 4: incompatible; cannot choose a single synchronized height
        else:
            sys.stdout.write("NO\n")
            return

    sys.stdout.write("YES\n")

if __name__ == "__main__":
    solve()
