import sys
from fractions import Fraction
from math import comb, factorial


def main():
    k, n = map(int, sys.stdin.read().split())
    m = n - k

    # D_m via inclusion-exclusion: D_m = sum_{i=0..m} (-1)^i * m!/i!,
    # independent of the reference's recurrence-based computation.
    d = sum((-1) ** i * factorial(m) // factorial(i) for i in range(m + 1))

    prob = Fraction(comb(n, k) * d, factorial(n))
    if prob == 0:
        print(0)
    else:
        print(f"{prob.numerator}/{prob.denominator}")


if __name__ == "__main__":
    main()
