import sys

sys.setrecursionlimit(1000000)


def rec(n, pos):
    if n == 1:
        return 1
    k = n // 2
    l = n - k

    if pos <= k:
        new_pos = k - pos + 1
        return l + rec(k, new_pos)
    else:
        new_pos = n - pos + 1
        return rec(l, new_pos)


def main():
    data = sys.stdin.read().split()
    N, q = map(int, data)
    print(rec(N, q))


if __name__ == "__main__":
    main()
