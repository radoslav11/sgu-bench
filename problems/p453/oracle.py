import math
import sys


def solve() -> None:
    # Read inputs
    l, v1, v2, T = map(int, sys.stdin.readline().split())

    # Relative speed when they move toward each other
    s = v1 + v2

    # Relative speed when they move in the same direction (catch-up)
    d = abs(v1 - v2)

    # Count type-1 meetings:
    # t = l*(2k+1)/s <= T  <=>  l*(2k+1) <= T*s
    # number of odd multiples of l up to T*s is floor((T*s + l)/(2*l))
    count1 = (T * s + l) // (2 * l)

    # Count type-2 meetings similarly, only if d>0
    count2 = (T * d + l) // (2 * l) if d > 0 else 0

    # Overlap of type-1 and type-2 sequences (avoid double counting)
    overlap = 0
    if d > 0:
        g = math.gcd(s, d)

        # Overlap exists only if (s/g) and (d/g) are both odd
        if ((s // g) % 2 == 1) and ((d // g) % 2 == 1):
            # Overlap times: t = (l/g) * odd
            # Count odd multiples of l/g within T:
            # floor((T*g + l)/(2*l))
            overlap = (T * g + l) // (2 * l)

    ans = count1 + count2 - overlap
    print(ans)


if __name__ == "__main__":
    solve()
