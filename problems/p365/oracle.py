import sys
from functools import lru_cache

def main():
    data = sys.stdin.read().strip().split()
    S = int(data[0])  # Length of the ticket number (1 <= S <= 20)

    # Digit DP with memoization.
    # State:
    #   pos   : how many digits have been placed (0..S)
    #   phase : 0 = non-decreasing phase, 1 = non-increasing phase
    #   last  : value of the last placed digit (0..9)
    @lru_cache(maxsize=None)
    def rec(pos: int, phase: int, last: int) -> int:
        # Base case: completed S digits -> count as 1 valid ticket
        if pos == S:
            return 1

        total = 0
        if phase == 0:
            # Still non-decreasing:
            # - choose digit >= last and stay in phase 0
            for d in range(last, 10):
                total += rec(pos + 1, 0, d)
            # - choose digit < last and switch to phase 1
            for d in range(0, last):
                total += rec(pos + 1, 1, d)
        else:
            # Non-increasing phase: must pick digit <= last
            for d in range(0, last + 1):
                total += rec(pos + 1, 1, d)

        return total

    # Start at position 0, in non-decreasing phase, with last=0
    # (leading zeros allowed so this enumerates all).
    ans = rec(0, 0, 0)
    print(ans)

if __name__ == "__main__":
    main()
