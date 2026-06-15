import sys

def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    # Sieve array: False means prime, True means composite
    is_composite = [False] * (n + 1)
    if n >= 0:
        is_composite[0] = True
    if n >= 1:
        is_composite[1] = True

    # Build sieve: mark composites
    for x in range(2, n + 1):
        if not is_composite[x]:
            # Mark multiples of x as composite
            for y in range(2 * x, n + 1, x):
                is_composite[y] = True

    result = []
    # Only pairs of the form (2, p) can sum to another prime
    # Check p from 2 to n-2
    for p in range(2, n - 1):
        if not is_composite[p] and not is_composite[p + 2]:
            # p is prime and p+2 is prime => 2+p is prime
            result.append((2, p))

    # Output
    out = [str(len(result))]
    for a, b in result:
        out.append(f"{a} {b}")
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
