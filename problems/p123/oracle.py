def main():
    import sys
    data = sys.stdin.read().strip().split()
    if not data:
        return
    k = int(data[0])  # Number of Fibonacci terms to sum

    # Handle small k directly
    if k == 1:
        print(1)
        return

    # Initialize first two Fibonacci numbers
    a, b = 1, 1
    total = a + b  # Sum of F1 and F2

    # Generate F3..Fk and accumulate their sum
    for _ in range(3, k + 1):
        c = a + b    # Next Fibonacci number
        total += c   # Add to running total
        a, b = b, c  # Shift for next iteration

    print(total)

if __name__ == "__main__":
    main()
