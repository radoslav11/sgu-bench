import cmath
import math
import sys


def main():
    data = sys.stdin.read().split()
    n, n1, n2 = int(data[0]), int(data[1]), int(data[2])
    p1 = complex(float(data[3]), float(data[4]))
    p2 = complex(float(data[5]), float(data[6]))

    # Clockwise numbering: vertex k sits at angle theta0 - 2*pi*(k-1)/n,
    # so p2 = C + (p1 - C) * w with w = exp(-i*2*pi*(n2-n1)/n).
    w = cmath.exp(-2j * math.pi * (n2 - n1) / n)
    c = (p1 * w - p2) / (w - 1)

    out = []
    for k in range(1, n + 1):
        v = c + (p1 - c) * cmath.exp(-2j * math.pi * (k - n1) / n)
        out.append(f"{v.real:.6f} {v.imag:.6f}")
    print("\n".join(out))


if __name__ == "__main__":
    main()
