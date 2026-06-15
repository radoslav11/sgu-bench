import sys


def ext_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x1, y1 = ext_gcd(b, a % b)
    return g, y1, x1 - (a // b) * y1


def count_k_in(lo, hi, base, step):
    # number of integers k with lo <= base + k*step <= hi, step != 0
    if step < 0:
        base, step = -base, -step
        lo, hi = -hi, -lo
    # k >= ceil((lo-base)/step), k <= floor((hi-base)/step)
    kmin = -((base - lo) // step)
    kmax = (hi - base) // step
    return kmin, kmax


def main():
    a, b, c, x1, x2, y1, y2 = map(int, sys.stdin.read().split())
    c = -c

    if a == 0 and b == 0:
        print((x2 - x1 + 1) * (y2 - y1 + 1) if c == 0 else 0)
        return

    if a == 0:
        if c % b == 0 and y1 <= c // b <= y2:
            print(x2 - x1 + 1)
        else:
            print(0)
        return

    if b == 0:
        if c % a == 0 and x1 <= c // a <= x2:
            print(y2 - y1 + 1)
        else:
            print(0)
        return

    g, x0, y0 = ext_gcd(abs(a), abs(b))
    if a < 0:
        x0 = -x0
    if b < 0:
        y0 = -y0

    if c % g != 0:
        print(0)
        return

    x0 *= c // g
    y0 *= c // g
    dx = b // g
    dy = -a // g

    kx_min, kx_max = count_k_in(x1, x2, x0, dx)
    ky_min, ky_max = count_k_in(y1, y2, y0, dy)

    lo = max(kx_min, ky_min)
    hi = min(kx_max, ky_max)
    print(max(0, hi - lo + 1))


if __name__ == "__main__":
    main()
