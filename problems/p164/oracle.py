import sys


def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    n = int(next(it))
    m = int(next(it))

    G = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            G[i][j] = int(next(it))

    for i in range(n):
        for j in range(n):
            G[i][j] %= 2

    def solve_even():
        INF = 10**9
        dist = [[INF] * n for _ in range(n)]
        for i in range(n):
            dist[i][i] = 0
        for i in range(n):
            for j in range(i + 1, n):
                if G[i][j] == 0:
                    dist[i][j] = 1
                    dist[j][i] = 1
        for k in range(n):
            dk = dist[k]
            for i in range(n):
                di = dist[i]
                ik = di[k]
                if ik == INF:
                    continue
                for j in range(n):
                    alt = ik + dk[j]
                    if alt < di[j]:
                        di[j] = alt
        for i in range(n):
            for j in range(n):
                if dist[i][j] > 3:
                    return False
        return True

    even_ok = solve_even()

    ans = []
    start = 2 if even_ok else 1
    for company in range(start, m + 1, 2):
        ans.append(company)

    out_lines = [str(len(ans)), " ".join(map(str, ans))]
    sys.stdout.write("\n".join(out_lines))


if __name__ == "__main__":
    solve()
