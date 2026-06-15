"""Second oracle for p294: Burnside via the divisor form of the sum.

Number of binary necklaces of length n = (1/n) * sum over d | n of
phi(d) * 2^(n/d). This is independent of the reference, which iterates
over all shifts i and groups them by gcd(n, i).
"""

import sys

sys.set_int_max_str_digits(1000000)


def divisors(n):
    small, large = [], []
    d = 1
    while d * d <= n:
        if n % d == 0:
            small.append(d)
            if d != n // d:
                large.append(n // d)
        d += 1
    return small + large[::-1]


def phi(n):
    result = n
    p = 2
    while p * p <= n:
        if n % p == 0:
            while n % p == 0:
                n //= p
            result -= result // p
        p += 1
    if n > 1:
        result -= result // n
    return result


def main():
    n = int(sys.stdin.read())
    total = 0
    for d in divisors(n):
        total += phi(d) * (1 << (n // d))
    print(total // n)


if __name__ == "__main__":
    main()
