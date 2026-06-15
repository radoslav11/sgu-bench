import sys


def apply_func(fn, x):
    L, R = fn
    lo, hi = (L, R) if L <= R else (R, L)
    if x < lo or x > hi:
        return x
    step = 1 if L <= R else -1
    if x != R:
        return x + step
    return 0


def solve():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    a = [int(next(it)) for _ in range(n)]

    if n == 1:
        print("FEDOR")
        return

    g = [[(0, 0) for _ in range(n)] for _ in range(n)]

    for length in range(3, n + 1):
        for l in range(0, n - length + 1):
            r = l + length - 1
            L = apply_func(g[l + 1][r], a[l + 1])
            inv_pair = (g[l][r - 1][1], g[l][r - 1][0])
            R = apply_func(inv_pair, a[r - 1])
            g[l][r] = (L, R)

    target = apply_func(g[0][n - 1], a[0])
    print("SERGEY" if target == a[n - 1] else "FEDOR")


if __name__ == "__main__":
    solve()
