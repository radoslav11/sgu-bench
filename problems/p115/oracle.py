import datetime
import sys


def main():
    n, m = map(int, sys.stdin.read().split())
    try:
        d = datetime.date(2001, m, n)
    except ValueError:
        print("Impossible")
        return
    print(d.isoweekday())


if __name__ == "__main__":
    main()
