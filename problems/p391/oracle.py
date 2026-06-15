import sys


def foldable_1d(v, lim):
    """
    Checks whether marked positions v in a 1D strip of length lim
    can be folded into one stack without including unmarked positions.

    v must be sorted and contain unique positions.
    """

    s = len(v)  # Number of marked positions.

    # Zero or one marked position is always foldable.
    if s < 2:
        return True

    # Consecutive marked positions must have odd distance.
    # If the distance is even, the structure cannot be folded correctly.
    for i in range(1, s):
        if (v[i] - v[i - 1]) % 2 == 0:
            return False

    # low and high delimit the still-relevant middle part of v.
    low = 0
    high = s

    # Current effective left and right borders of the strip.
    left_border = 1
    right_border = lim

    # Remove consecutive marked cells touching the left border.
    while low < high and v[low] == left_border:
        low += 1
        left_border += 1

    # Remove consecutive marked cells touching the right border.
    while low < high and v[high - 1] == right_border:
        high -= 1
        right_border -= 1

    # seq stores alternating:
    # run length of consecutive marked cells,
    # gap between this run and the next run.
    seq = []

    i = low

    # Split the remaining marked positions into consecutive runs.
    while i < high:
        j = i

        # Find the end of the current consecutive marked run.
        while j + 1 < high and v[j + 1] == v[j] + 1:
            j += 1

        # Length of current consecutive marked run.
        run_len = j - i + 1

        # Internal runs longer than one must have even length.
        if run_len > 1 and run_len % 2 == 1:
            return False

        # Store current run length.
        seq.append(run_len)

        # If another run follows, store the distance to the next run.
        if j + 1 < high:
            seq.append(v[j + 1] - v[j])

        # Move to the next run.
        i = j + 1

    # prev stores the index in seq of the previous isolated single run.
    prev = -1

    # Run lengths are located at even indices in seq.
    for idx in range(0, len(seq), 2):
        # Only isolated single-cell runs require palindrome checks.
        if seq[idx] != 1:
            continue

        # If there was a previous isolated run, check symmetry between them.
        if prev >= 0:
            l = prev + 1
            r = idx - 1

            # The pattern between two isolated marks must be palindromic.
            while l < r:
                if seq[l] != seq[r]:
                    return False
                l += 1
                r -= 1

        # Remember this isolated single run.
        prev = idx

    # All checks passed.
    return True


def is_full_grid(cells, cols):
    """
    Checks whether sorted marked cells form exactly R x cols,
    where R is the set of distinct rows appearing in cells.

    cells must be sorted lexicographically by row and column.
    cols must be sorted unique columns.
    """

    n_cells = len(cells)  # Number of marked cells.
    n_cols = len(cols)    # Number of distinct marked columns.

    # No marked cells: trivially okay.
    if n_cells == 0:
        return True

    # If there are marked cells but no columns, impossible.
    # This is mostly defensive; with valid input this should not happen.
    if n_cols == 0:
        return False

    # A full grid must contain the same number of columns in every row.
    if n_cells % n_cols != 0:
        return False

    # Number of rows in the supposed full product.
    n_rows = n_cells // n_cols

    # Check each row block.
    for row_idx in range(n_rows):
        # First cell of this block determines the expected row value.
        expected_row = cells[row_idx * n_cols][0]

        # Every column must appear exactly once in this row.
        for col_idx in range(n_cols):
            cell = cells[row_idx * n_cols + col_idx]

            # The row and column must match the expected Cartesian product.
            if cell[0] != expected_row or cell[1] != cols[col_idx]:
                return False

    # All rows contain exactly all columns.
    return True


def solve():
    # Read all integers from standard input.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # If input is empty, do nothing.
    if not data:
        return

    # First three integers are n, m, k.
    n, m, k = data[0], data[1], data[2]

    # Remaining data contains k pairs.
    pts = []

    index = 3

    # Read marked cells.
    for _ in range(k):
        x = data[index]
        y = data[index + 1]
        index += 2
        pts.append((x, y))

    # Extract distinct marked rows and columns.
    rows = sorted(set(x for x, y in pts))
    cols = sorted(set(y for x, y in pts))

    # Row set and column set must be independently foldable.
    if not foldable_1d(rows, n) or not foldable_1d(cols, m):
        print("NO")
        return

    # Sort cells by row, then column.
    pts.sort()

    # Marked cells must form the complete Cartesian product rows x cols.
    if not is_full_grid(pts, cols):
        print("NO")
        return

    # All conditions passed.
    print("YES")


if __name__ == "__main__":
    solve()
