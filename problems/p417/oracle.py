import math
import sys


def main():
    data = sys.stdin.read().split()
    x0, y0, r = map(int, data)

    s2 = x0 * x0 + y0 * y0
    mass = math.pi * (r**2) * math.log(s2)

    print("{:.12f}".format(mass))


if __name__ == "__main__":
    main()
