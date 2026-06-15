import sys

def compute_phi(n):
    result = n
    x = n
    p = 2
    # Trial divide up to sqrt(x)
    while p * p <= x:
        if x % p == 0:
            # p is a prime divisor
            while x % p == 0:
                x //= p
            # apply totient factor (1 - 1/p)
            result -= result // p
        p += 1 if p == 2 else 2  # increment p (2→3, then skip evens)
    # If remainder x > 1, it's a prime factor
    if x > 1:
        result -= result // x
    return result

def main():
    n = int(sys.stdin.readline())
    print(compute_phi(n))

if __name__ == "__main__":
    main()
