import sys

def solve() -> None:
    # Read N and K
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    k = int(data[1])

    size = n * n  # The grid is (N^2) x (N^2)

    # Initialize the full grid with '.' (red cells).
    # We'll build it as a list of lists for easy assignment,
    # then convert rows to strings at the end.
    grid = [['.' for _ in range(size)] for _ in range(size)]

    # Step 1: fill the base (top-left) N x N block with exactly K stars.
    # Place stars in row-major order among the first N*N cells.
    for c in range(k):
        r = c // n     # row inside base block
        col = c % n    # col inside base block
        grid[r][col] = '*'

    # Step 2: fill every other block (br, bc) with a cyclic shift of the base block.
    for br in range(n):           # block row
        for bc in range(n):       # block column
            if br == 0 and bc == 0:
                continue          # base block already done

            # Fill the N x N cells of this block
            for i in range(n):    # inner row
                for j in range(n):# inner col
                    # Destination in big grid:
                    R = br * n + i
                    C = bc * n + j

                    # Source from base block with cyclic shift:
                    src_r = (i - bc) % n
                    src_c = (j - br) % n

                    grid[R][C] = grid[src_r][src_c]

    # Print the grid
    out_lines = [''.join(row) for row in grid]
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
