import sys

input = sys.stdin.readline


def main():
    n, m = map(int, input().split())
    rows = [(1 << n) - 1 for _ in range(n)]

    for _ in range(m):
        x1, y1, x2, y2, c = input().split()
        x1 = int(x1) - 1
        y1 = int(y1) - 1
        x2 = int(x2) - 1
        y2 = int(y2) - 1
        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1

        mask = ((1 << (y2 - y1 + 1)) - 1) << y1

        if c == 'w':
            for i in range(x1, x2 + 1):
                rows[i] |= mask
        else:
            comp = ~mask
            for i in range(x1, x2 + 1):
                rows[i] &= comp

    print(sum(r.bit_count() for r in rows))


if __name__ == '__main__':
    main()
