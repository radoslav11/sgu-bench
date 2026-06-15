import sys


def rotate_k(r, c, k):
    """
    Rotate point (r, c) by k * 90 degrees around the origin.

    k = 0: (r, c)
    k = 1: (c, -r)
    k = 2: (-r, -c)
    k = 3: (-c, r)
    """
    for _ in range(k):
        r, c = c, -r
    return r, c


def solve():
    data = sys.stdin.read().strip().split()

    # Read N.
    n = int(data[0])

    # Read grid.
    grid = data[1:1 + n]

    # Collect all cells belonging to Q.
    cells = []
    for r in range(n):
        for c in range(n):
            if grid[r][c] == '1':
                cells.append((r, c))

    total = len(cells)

    # Empty set is trivially splittable.
    if total == 0:
        print("YES")
        for _ in range(n):
            print("0" * n)
        return

    # Two congruent parts must have equal size.
    if total % 2 == 1:
        print("NO")
        return

    # Check whether a cell is inside Q.
    def in_q(r, c):
        return 0 <= r < n and 0 <= c < n and grid[r][c] == '1'

    # Use the first marked cell as anchor.
    c0r, c0c = cells[0]

    def try_transform(k, dr, dc):
        """
        Try transformation:
            f(x) = rotate_k(x, k) + (dr, dc)

        Return the resulting subset grid if valid, otherwise None.
        """

        def f(r, c):
            # Apply rotation.
            nr, nc = rotate_k(r, c, k)

            # Apply translation.
            return nr + dr, nc + dc

        def f_inv(r, c):
            # Undo translation.
            r -= dr
            c -= dc

            # Undo rotation.
            return rotate_k(r, c, (4 - k) % 4)

        # color[r][c]:
        # -1 means unvisited,
        #  0 means chosen subset A,
        #  1 means transformed subset B.
        color = [[-1] * n for _ in range(n)]

        # Process every marked cell.
        for i in range(n):
            for j in range(n):
                # Skip cells not in Q or already visited.
                if grid[i][j] != '1' or color[i][j] != -1:
                    continue

                # Find start of the component.
                sr, sc = i, j

                while True:
                    pr, pc = f_inv(sr, sc)

                    # If predecessor is outside Q, this is path start.
                    # If predecessor is the original cell, this is a cycle.
                    if not in_q(pr, pc) or (pr == i and pc == j):
                        break

                    sr, sc = pr, pc

                # Traverse forward and color alternately.
                cr, cc = sr, sc
                current_color = 0
                length = 0

                while color[cr][cc] == -1:
                    # Assign color.
                    color[cr][cc] = current_color

                    # Alternate between 0 and 1.
                    current_color ^= 1

                    # Count component size.
                    length += 1

                    # Move to next cell under f.
                    nr, nc = f(cr, cc)

                    # Stop if next cell is outside Q.
                    if not in_q(nr, nc):
                        break

                    cr, cc = nr, nc

                # Odd component cannot be alternately split.
                if length % 2 == 1:
                    return None

        # Build output subset: cells colored 0.
        answer = []
        for r in range(n):
            row = []
            for c in range(n):
                if grid[r][c] == '1' and color[r][c] == 0:
                    row.append('1')
                else:
                    row.append('0')
            answer.append(''.join(row))

        return answer

    # Try all four rotations.
    for k in range(4):
        # Anchor after rotation.
        rr, rc = rotate_k(c0r, c0c, k)

        # Try mapping anchor to every other marked cell.
        for tr, tc in cells:
            if tr == c0r and tc == c0c:
                continue

            # Translation needed.
            dr = tr - rr
            dc = tc - rc

            result = try_transform(k, dr, dc)

            # If valid, print answer.
            if result is not None:
                print("YES")
                print("\n".join(result))
                return

    # Nothing worked.
    print("NO")


if __name__ == "__main__":
    solve()
