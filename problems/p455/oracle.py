"""Independent oracle for p455 (Sequence analysis).

Stores the first index of every value in a dict; the first duplicate hit
while scanning x_0, x_1, ... is exactly the second occurrence of the
first repeatable element (rho-shaped orbit). Arithmetic emulates C++
int64 exactly: products/sums wrap modulo 2^64 and '%' truncates toward
zero (sign of the dividend), unlike Python's floored '%'.
"""

import sys


def main():
    A, B, C = map(int, sys.stdin.read().split())
    FULL = 1 << 64
    HALF = 1 << 63

    def wrap(v):
        v &= FULL - 1
        return v - FULL if v >= HALF else v

    def cmod(a, b):
        r = abs(a) % b
        return -r if a < 0 else r

    seen = {}
    x = 1
    for i in range(2_000_001):
        if x in seen:
            print(i)
            return
        seen[x] = i
        x = cmod(wrap(wrap(A * x) + cmod(x, B)), C)
    print(-1)


if __name__ == "__main__":
    main()
