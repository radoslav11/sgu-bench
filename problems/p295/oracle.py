import sys
import math

def main() -> None:
    # Read q, c
    line1 = sys.stdin.readline().strip()
    q, c = map(int, line1.split())

    # Read nickname line (may contain spaces, may be empty but per statement it’s a nickname)
    s = sys.stdin.readline().rstrip("\n")

    # Set of Latin letters that have Russian look-alikes
    amb = set("ABCEHKMOPTXaceopxy")

    # Split into words by spaces (consecutive spaces collapse)
    # This matches the C++ manual split behavior.
    words = [w for w in s.split(' ') if w != '']
    w = len(words)

    # Total non-space length L and ambiguous count A
    L = sum(len(word) for word in words)
    A = sum(1 for word in words for ch in word if ch in amb)

    # Letter variants: 2^A
    letter_ways = 1 << A  # Python big int

    # Valid total spaces S range
    s_min = max(0, w - 1, q - L)
    s_max = c - L

    if s_min > s_max:
        print(0)
        return

    # For fixed S, spacing variants = C(S+1, w)
    # Sum these over S in [s_min, s_max].
    space_ways = 0
    for S in range(s_min, s_max + 1):
        space_ways += math.comb(S + 1, w)

    # Total = letter_ways * space_ways, exclude the original nickname itself
    ans = letter_ways * space_ways - 1
    print(ans)

if __name__ == "__main__":
    main()
