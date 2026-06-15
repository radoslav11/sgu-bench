import sys

def main() -> None:
    # Read N and M
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    m = int(data[1])

    # Python's int is arbitrary-precision, so we can compute powers directly.
    # Number of valid configurations:
    #   answer = 2^n + 2^m - 2
    # The "-2" removes the two checkerboard patterns counted in both groups.
    ans = (1 << n) + (1 << m) - 2

    # Print as a normal decimal integer (no leading zeros).
    sys.stdout.write(str(ans))

if __name__ == "__main__":
    main()
