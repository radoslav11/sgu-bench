import sys

def main():
    data = sys.stdin.read().split()
    n, k = map(int, data)
    # Base value for each position
    a = k // n
    # Remainder to distribute
    d = k % n

    # Initialize all positions with a
    ans = [a] * n

    # Find t such that d * t ≡ n-1 (mod n)
    # Since gcd(n,d)=1, such t exists uniquely
    for t in range(1, n):
        if (d * t) % n == (n - 1):
            # Distribute the d extra ones around the cycle
            x = 0
            # We must do exactly d increments; the cycle length is d+1 steps to land on n-1
            while True:
                x = (x + t) % n
                ans[x] += 1
                if x == n - 1:
                    break
            break

    # Print the result
    print(" ".join(map(str, ans)))

if __name__ == "__main__":
    main()
