import sys


def main():
    # Read a single integer x from standard input
    data = sys.stdin.read().strip()
    x = int(data)

    # Derivation shows the answer is N = 2*x + 1
    result = 2 * x + 1

    # Print the result
    print(result)


if __name__ == "__main__":
    main()
