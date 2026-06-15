import sys


def main():
    data = sys.stdin.read().split()
    n, m, k = int(data[0]), int(data[1]), int(data[2])
    vals = [int(x) for x in data[3:3 + n]]
    print(sum(1 for v in vals if pow(v, m, k) == 0))


if __name__ == "__main__":
    main()
