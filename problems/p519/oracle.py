import sys


def main():
    data = sys.stdin.read().split()
    n, m = int(data[0]), int(data[1])
    grid = data[2 : 2 + n]

    total = 0
    dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    for i in range(n):
        for j in range(m):
            h = int(grid[i][j])
            if h == 0:
                continue
            total += 2
            for dx, dy in dirs:
                ni, nj = i + dx, j + dy
                hn = int(grid[ni][nj]) if 0 <= ni < n and 0 <= nj < m else 0
                if h > hn:
                    total += h - hn

    print(total)


if __name__ == "__main__":
    main()
