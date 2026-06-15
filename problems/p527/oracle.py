import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    rows = [data[2 + i].decode() for i in range(n)]

    grid = [[1 if (j < len(rows[i]) and rows[i][j] == '*') else 0
             for j in range(m)] for i in range(n)]

    if not any(any(r) for r in grid):
        print(0)
        return

    # A bomb at (i, j) clears all rocks in row i and column j. With k bombs we
    # can cover up to k rows and k columns, so the answer is
    #   min over a chosen row set R of max(|R|, |columns needed for rows not
    #   in R|),
    # where the needed columns are those holding a rock in some row outside R.
    # Enumerate subsets over the smaller dimension for speed.
    transpose = m < n
    if transpose:
        grid = [[grid[i][j] for i in range(n)] for j in range(m)]
        n, m = m, n

    row_mask = [0] * n
    for i in range(n):
        for j in range(m):
            if grid[i][j]:
                row_mask[i] |= 1 << j

    FULL = (1 << n) - 1
    union = [0] * (1 << n)
    for i in range(n):
        union[1 << i] = row_mask[i]
    for s in range(1, 1 << n):
        low = s & -s
        if s != low:
            union[s] = union[low] | union[s ^ low]

    best = n + m
    for s in range(1 << n):
        rcount = bin(s).count("1")
        ccount = bin(union[FULL ^ s]).count("1")
        cost = rcount if rcount >= ccount else ccount
        if cost < best:
            best = cost

    print(best)


if __name__ == "__main__":
    main()
