import sys
import math

def compute_partitions(max_n):
    """
    Compute partition numbers p(0..max_n) using Euler's
    recurrence with generalized pentagonal numbers.
    """
    p = [0] * (max_n + 1)
    p[0] = 1  # Base case

    for m in range(1, max_n + 1):
        total = 0
        k = 1
        while True:
            # Generalized pentagonal numbers:
            pent1 = k * (3*k - 1) // 2
            pent2 = k * (3*k + 1) // 2
            if pent1 > m and pent2 > m:
                break
            sign = 1 if (k % 2 == 1) else -1

            if pent1 <= m:
                total += sign * p[m - pent1]
            if pent2 <= m:
                total += sign * p[m - pent2]

            k += 1
        p[m] = total

    return p

def main():
    # Read n from stdin
    n = int(sys.stdin.readline())
    temp = n
    result = 1

    # Factor n by trial division
    # Keep track of maximum exponent to know how far to compute partitions
    exponents = []
    # Check divisor 2 separately for slight speedup
    if temp % 2 == 0:
        cnt = 0
        while temp % 2 == 0:
            temp //= 2
            cnt += 1
        exponents.append(cnt)

    # Check odd divisors up to sqrt(temp)
    f = 3
    while f * f <= temp:
        if temp % f == 0:
            cnt = 0
            while temp % f == 0:
                temp //= f
                cnt += 1
            exponents.append(cnt)
        f += 2

    # If remainder > 1, it's a prime factor with exponent 1
    if temp > 1:
        exponents.append(1)

    # Precompute partitions up to the largest exponent
    max_e = max(exponents, default=0)
    partitions = compute_partitions(max_e)

    # Multiply partition(exponent) for each prime's exponent
    for e in exponents:
        result *= partitions[e]

    # Print the result
    print(result)

if __name__ == "__main__":
    main()
