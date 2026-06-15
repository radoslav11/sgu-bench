import math
from math import comb
from fractions import Fraction

def solve():
    # Read n (0..50)
    n = int(input().strip())

    # We use the recurrence:
    #   (3*2^m - 2) * I_m = 1 + sum_{k=0..m-1} C(m,k) * I_k
    # with I_0 = 1.
    #
    # In Python, integers are arbitrary precision, so we can safely store
    # huge numerators/denominators. We'll still do explicit gcd reductions.

    # Store I_k as (p_k, q_k) in lowest terms.
    p = [0] * (n + 1)
    q = [0] * (n + 1)

    p[0], q[0] = 1, 1  # I_0 = 1

    # common will track lcm(q_0, ..., q_{m-1}) to add rationals efficiently
    common = 1

    for m in range(1, n + 1):
        # num/common will represent:
        # 1 + sum_{k=0..m-1} C(m,k) * (p[k]/q[k])
        #
        # The "1" contributes common/common.
        num = common

        for k in range(m):
            # Add C(m,k) * p[k]/q[k] in terms of denominator = common:
            # numerator contribution = (common // q[k]) * p[k] * C(m,k)
            num += (common // q[k]) * p[k] * comb(m, k)

        # Now I_m = num / ( common * (3*2^m - 2) )
        den = common * (3 * (1 << m) - 2)

        # Reduce the fraction once
        g = math.gcd(num, den)
        p[m] = num // g
        q[m] = den // g

        # Update common = lcm(common, q[m])
        common = common // math.gcd(common, q[m]) * q[m]

    print(f"{p[n]}/{q[n]}")

if __name__ == "__main__":
    solve()
