import sys
import math

def prime_factors(n):
    """Return the distinct prime factors of n."""
    factors = []
    # Check divisibility by 2 separately for speed (optional)
    if n % 2 == 0:
        factors.append(2)
        while n % 2 == 0:
            n //= 2
    # Now n is odd; test odd divisors from 3 up to sqrt(n)
    i = 3
    # Stop when i*i > n
    while i * i <= n:
        if n % i == 0:
            factors.append(i)
            # Remove all copies of i
            while n % i == 0:
                n //= i
        i += 2
    # If remainder > 1, it's a prime
    if n > 1:
        factors.append(n)
    return factors

def key_number(n):
    """Compute key(n) = 2*max(primes) - sum(primes)."""
    primes = prime_factors(n)
    s = sum(primes)
    m = max(primes)
    return 2*m - s

def main():
    # Read two integers from stdin
    a, b = map(int, sys.stdin.readline().split())
    # Compare keys and print result
    if key_number(a) > key_number(b):
        print("a")
    else:
        print("b")

if __name__ == "__main__":
    main()
