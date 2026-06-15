import sys
from fractions import Fraction


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx])
    idx += 1
    circ = []
    for _ in range(n):
        x = int(data[idx])
        y = int(data[idx + 1])
        r = int(data[idx + 2])
        idx += 3
        circ.append((x, y, r))

    # Each orthogonality condition is linear in (X, Y, Z) with Z = X^2+Y^2-R^2:
    #   (-2 xi) X + (-2 yi) Y + Z = ri^2 - xi^2 - yi^2.
    # Solve the system exactly with rational Gaussian elimination so the
    # rank / consistency decision is unambiguous.
    rows = []
    for (x, y, r) in circ:
        rows.append([
            Fraction(-2 * x),
            Fraction(-2 * y),
            Fraction(1),
            Fraction(r * r - x * x - y * y),
        ])

    cols = 3
    pivot = 0
    pivot_cols = []
    m = len(rows)
    for col in range(cols):
        sel = -1
        for row in range(pivot, m):
            if rows[row][col] != 0:
                sel = row
                break
        if sel == -1:
            continue
        rows[pivot], rows[sel] = rows[sel], rows[pivot]
        div = rows[pivot][col]
        rows[pivot] = [v / div for v in rows[pivot]]
        for row in range(m):
            if row != pivot and rows[row][col] != 0:
                f = rows[row][col]
                rows[row] = [a - f * b for a, b in zip(rows[row], rows[pivot])]
        pivot_cols.append(col)
        pivot += 1

    # Inconsistent row: 0 = nonzero.
    for row in range(pivot, m):
        if rows[row][3] != 0:
            print(-1)
            return

    if pivot < 3:
        print(-2)
        return

    X = rows[0][3]
    Y = rows[1][3]
    Z = rows[2][3]
    r_sq = X * X + Y * Y - Z
    if r_sq <= 0:
        print(-1)
        return

    import math

    rr = math.sqrt(float(r_sq))
    print("%.10f %.10f %.10f" % (float(X), float(Y), rr))


if __name__ == "__main__":
    main()
