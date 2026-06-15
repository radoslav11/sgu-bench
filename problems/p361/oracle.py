def main():
    import sys
    input = sys.stdin.readline

    # Read N, M
    n, m = map(int, input().split())

    best_offset = 0
    min_count = n * m + 1  # initialize to larger than maximum possible

    # Find which offset yields fewest blue cells
    for offset in range(3):
        cnt = 0
        for i in range(n):
            # Instead of checking every j, we can note that for fixed i,
            # j runs from 0 to m-1. We still check each one here for clarity.
            for j in range(m):
                if (i + j + offset) % 3 == 0:
                    cnt += 1
        if cnt < min_count:
            min_count = cnt
            best_offset = offset

    # Build the grid using the optimal offset
    grid = []
    for i in range(n):
        row = []
        for j in range(m):
            # Mark '#' if it satisfies the chosen diagonal pattern
            if (i + j + best_offset) % 3 == 0:
                row.append('#')
            else:
                row.append('0')
        grid.append(''.join(row))

    # Print the resulting flag
    print('\n'.join(grid))


if __name__ == "__main__":
    main()
