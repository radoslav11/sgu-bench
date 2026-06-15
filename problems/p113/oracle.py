import sys


def is_prime(x):
    if x < 2:
        return False
    d = 2
    while d * d <= x:
        if x % d == 0:
            return False
        d += 1
    return True


def nearly_prime(x):
    d = 2
    while d * d <= x:
        if x % d == 0:
            return is_prime(x // d)
        d += 1
    return False


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    for tok in data[1:1 + n]:
        print("Yes" if nearly_prime(int(tok)) else "No")


if __name__ == "__main__":
    main()
