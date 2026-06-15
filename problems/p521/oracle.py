import sys
from collections import defaultdict


class FenwickMax:
    def __init__(self, n):
        self.n = n
        self.fw = [0] * (n + 1)

    def update(self, i, v):
        while i <= self.n:
            if self.fw[i] < v:
                self.fw[i] = v
            i += i & -i

    def query(self, i):
        res = 0
        while i > 0:
            if self.fw[i] > res:
                res = self.fw[i]
            i -= i & -i
        return res


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    xs = [0] * n
    ys = [0] * n
    for i in range(n):
        xs[i] = int(data[idx]); idx += 1
        ys[i] = int(data[idx]); idx += 1

    ys_sorted = sorted(set(ys))
    m = len(ys_sorted)
    rank = {y: r + 1 for r, y in enumerate(ys_sorted)}
    ry = [rank[ys[i]] for i in range(n)]

    order = sorted(range(n), key=lambda i: xs[i])
    up_dp = [0] * n
    ft = FenwickMax(m)
    i = 0
    while i < n:
        x0 = xs[order[i]]
        batch = []
        while i < n and xs[order[i]] == x0:
            batch.append(order[i])
            i += 1
        for p in batch:
            up_dp[p] = 1 + ft.query(ry[p] - 1)
        for p in batch:
            ft.update(ry[p], up_dp[p])

    order = sorted(range(n), key=lambda i: -xs[i])
    down_dp = [0] * n
    ft = FenwickMax(m)
    i = 0
    while i < n:
        x0 = xs[order[i]]
        batch = []
        while i < n and xs[order[i]] == x0:
            batch.append(order[i])
            i += 1
        for p in batch:
            mry = m + 1 - ry[p]
            down_dp[p] = 1 + ft.query(mry - 1)
        for p in batch:
            mry = m + 1 - ry[p]
            ft.update(mry, down_dp[p])

    L = 0
    for i in range(n):
        v = up_dp[i] + down_dp[i] - 1
        if v > L:
            L = v

    A = [i + 1 for i in range(n) if up_dp[i] + down_dp[i] - 1 == L]
    bylev = defaultdict(list)
    for idx1 in A:
        bylev[up_dp[idx1 - 1]].append(idx1)
    B = sorted(v[0] for v in bylev.values() if len(v) == 1)

    out = []
    out.append(" ".join(map(str, [len(A)] + A)))
    out.append(" ".join(map(str, [len(B)] + B)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
