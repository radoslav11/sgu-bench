"""Second oracle for p282: Burnside over cycle types of S_n.

Partitions of n are enumerated as (length, multiplicity) pairs; for each
cycle type the number of edge orbits is sum(c//2) + sum over pairs of
cycles gcd(ci, cj), and the orbit count of the conjugacy class divided
by n! is 1 / (prod ci * prod mult!).
"""

import sys
from math import gcd


def main():
    n, m, p = map(int, sys.stdin.read().split())

    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % p

    total = 0
    # cur: list of (part_length, multiplicity), non-increasing lengths
    stack = [(n, n, [])]
    while stack:
        last, left, cur = stack.pop()
        if left == 0:
            ex = 0
            denom = 1
            k = len(cur)
            for i in range(k):
                a, ka = cur[i]
                ex += (a // 2) * ka
                if a > 1:
                    ex += a * (ka * (ka - 1) // 2)
                else:
                    ex += ka * (ka - 1) // 2
                for j in range(i + 1, k):
                    b, kb = cur[j]
                    ex += gcd(a, b) * ka * kb
                denom = denom * pow(a, ka, p) % p * fact[ka] % p
            total = (total + pow(m, ex, p) * pow(denom, p - 2, p)) % p
            continue
        for x in range(min(last, left), 0, -1):
            for k in range(1, left // x + 1):
                stack.append((x - 1, left - k * x, cur + [(x, k)]))

    print(total % p)


main()
