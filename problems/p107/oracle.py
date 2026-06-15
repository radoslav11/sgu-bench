import sys


def main():
    n = int(sys.stdin.read().strip())

    # Squares ending in 987654321 have 9-digit tails counted by brute
    # force over the last 9 digits: 8 such tails, each extendable by any
    # leading digits; the leading digit of an N-digit number cannot be 0.
    if n < 9:
        print(0)
    elif n == 9:
        print(8)
    else:
        print("72" + "0" * (n - 10))


if __name__ == "__main__":
    main()
