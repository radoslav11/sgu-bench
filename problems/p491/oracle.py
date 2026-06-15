import sys


def solve():
    n = int(sys.stdin.readline())
    divisors = [[] for _ in range(n + 1)]
    for d in range(1, n + 1):
        for m in range(d, n + 1, d):
            divisors[m].append(d)

    visited = [0] * (n + 1)
    ans = 0
    for a in range(1, n):
        ax = a
        while ax < n:
            r = n - ax
            va = a
            for b in divisors[r]:
                if b > a and visited[b] != va:
                    visited[b] = va
                    ans += 1
            ax += a
    print(ans)


if __name__ == "__main__":
    solve()
