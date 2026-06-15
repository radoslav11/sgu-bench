import sys
from math import gcd


def game_value_fraction(n):
    if n == 1:
        return -1, 1

    if n % 2 == 1:
        numerator = 3 - n
        denominator = n + 1
    elif n % 4 == 0:
        numerator = 4 - n
        denominator = n
    else:
        numerator = 2 - n
        denominator = n + 2

    g = gcd(abs(numerator), denominator)
    return numerator // g, denominator // g


def main():
    data = sys.stdin.read().split()
    for case_id, token in enumerate(data, start=1):
        numerator, denominator = game_value_fraction(int(token))
        print(f"Case #{case_id}: {numerator}/{denominator}")


if __name__ == "__main__":
    main()
