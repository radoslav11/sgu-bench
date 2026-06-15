import sys


def bareiss_det(matrix):
    """
    Computes the exact integer determinant of a square matrix.

    Bareiss algorithm is a fraction-free version of Gaussian elimination.
    It keeps all intermediate values as integers and has O(n^3) complexity.

    For n <= 50, this is easily practical in Python.
    """

    # Make a deep copy, because the algorithm modifies the matrix.
    a = [row[:] for row in matrix]

    # Matrix size.
    n = len(a)

    # The determinant sign changes whenever we swap two rows.
    sign = 1

    # Previous pivot used for exact division.
    # In Bareiss algorithm, this starts as 1.
    prev = 1

    # Process columns 0 through n-2.
    # After that, the last diagonal value contains the determinant up to sign.
    for k in range(n - 1):
        # Find a nonzero pivot in column k at or below row k.
        pivot_row = k
        while pivot_row < n and a[pivot_row][k] == 0:
            pivot_row += 1

        # If no pivot exists, determinant is zero.
        if pivot_row == n:
            return 0

        # Swap pivot row into position k if needed.
        if pivot_row != k:
            a[k], a[pivot_row] = a[pivot_row], a[k]
            sign *= -1

        # Current pivot.
        pivot = a[k][k]

        # Update the remaining submatrix.
        for i in range(k + 1, n):
            for j in range(k + 1, n):
                # Bareiss update:
                #
                # a[i][j] = (a[i][j] * pivot - a[i][k] * a[k][j]) / prev
                #
                # The division is exact for integer matrices.
                a[i][j] = (a[i][j] * pivot - a[i][k] * a[k][j]) // prev

        # Entries below the pivot are no longer needed.
        for i in range(k + 1, n):
            a[i][k] = 0

        # Store this pivot for the next Bareiss division.
        prev = pivot

    # For a 1x1 matrix, determinant is just its only element.
    # The problem has n >= 2, but this keeps the function general.
    if n == 1:
        return a[0][0]

    # Last diagonal entry is determinant up to row-swap sign.
    return sign * a[n - 1][n - 1]


def main():
    # Read all input tokens.
    data = sys.stdin.buffer.read().split()

    # First token is n, second is p.
    n = int(data[0])
    p = int(data[1])

    # Read the n by n matrix.
    values = list(map(int, data[2:]))

    matrix = []
    index = 0

    for _ in range(n):
        row = values[index:index + n]
        matrix.append(row)
        index += n

    # Compute exact determinant.
    det = bareiss_det(matrix)

    # We need |det| modulo p.
    abs_det_mod = abs(det) % p

    # Compute 2^n modulo p.
    pow2 = pow(2, n, p)

    # Final formula.
    answer = abs_det_mod * pow2 % p

    # Print answer.
    print(answer)


if __name__ == "__main__":
    main()
