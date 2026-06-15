import sys


def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    a = [int(next(it)) for _ in range(n)]

    dp = [[False] * m for _ in range(n + 1)]
    take = [[False] * m for _ in range(n + 1)]

    dp[0][1] = True

    for i in range(n):
        ai = a[i] % m
        row = dp[i]
        nxt = dp[i + 1]
        tk = take[i + 1]
        for r in range(m):
            if not row[r]:
                continue
            nxt[r] = True
            nr = (r * ai) % m
            nxt[nr] = True
            tk[nr] = True

    max_r = 0
    for r in range(m - 1, -1, -1):
        if dp[n][r]:
            max_r = r
            break

    res = []
    curr = max_r
    for i in range(n, 0, -1):
        if not take[i][curr]:
            continue
        res.append(i)
        ai = a[i - 1] % m
        for prev in range(m):
            if dp[i - 1][prev] and (prev * ai) % m == curr:
                curr = prev
                break

    res.reverse()

    sys.stdout.write(str(max_r) + "\n")
    if res:
        sys.stdout.write(" ".join(map(str, res)) + "\n")


if __name__ == "__main__":
    main()
