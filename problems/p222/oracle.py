import math
import sys


def main():
    data = sys.stdin.read().split()
    n, k = map(int, data)

    # if k > n, cannot place k non-attacking rooks
    if k > n:
        print(0)
        return

    # number of ways = C(n,k) * C(n,k) * k!
    cnk = math.comb(n, k)
    print(cnk * cnk * math.factorial(k))


if __name__ == "__main__":
    main()
