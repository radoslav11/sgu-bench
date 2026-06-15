import sys
import math


def determinant_gauss(a):
    n = len(a)
    mat = [row[:] for row in a]
    det = 1.0
    for i in range(n):
        pivot = i
        for r in range(i + 1, n):
            if abs(mat[r][i]) > abs(mat[pivot][i]):
                pivot = r
        if abs(mat[pivot][i]) < 1e-12:
            return 0.0
        if pivot != i:
            mat[i], mat[pivot] = mat[pivot], mat[i]
            det = -det
        pivot_val = mat[i][i]
        det *= pivot_val
        for r in range(i + 1, n):
            factor = mat[r][i] / pivot_val
            for c in range(i, n):
                mat[r][c] -= factor * mat[i][c]
    return det


def main():
    data = sys.stdin.read().strip().split()
    AB, AC, AD, BC, BD, CD = map(float, data)

    a2 = AB * AB
    b2 = AC * AC
    c2 = AD * AD
    d2 = BC * BC
    e2 = BD * BD
    f2 = CD * CD

    M = [
        [0.0, 1.0, 1.0, 1.0, 1.0],
        [1.0, 0.0, a2, b2, c2],
        [1.0, a2, 0.0, d2, e2],
        [1.0, b2, d2, 0.0, f2],
        [1.0, c2, e2, f2, 0.0],
    ]

    det = determinant_gauss(M)
    volume = math.sqrt(abs(det) / 288.0)
    print(f"{volume:.4f}")


if __name__ == "__main__":
    main()
