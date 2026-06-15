def main():
    import sys
    data = sys.stdin.read().split()
    k = int(data[0])  # number of chord pairs

    # Initialize a list for Catalan numbers up to k
    C = [0] * (k + 1)
    C[0] = 1  # base case

    # Build up using C[n] = sum_{i=0..n-1} C[i]*C[n-1-i]
    for n in range(1, k + 1):
        total = 0
        for i in range(n):
            total += C[i] * C[n - 1 - i]
        C[n] = total

    # Minimal regions = k+1
    print(C[k], k + 1)

if __name__ == "__main__":
    main()
