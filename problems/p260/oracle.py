import sys

# Solve linear system over GF(2) using integer bitmasks.
# Since N <= 200, we can store each row as an int with (N+1) bits:
# bits [0..N-1] are coefficients, bit N is RHS.
def gauss_xor(rows, n):
    """
    rows: list[int], each is bitmask of length n+1 (coeffs + rhs at bit n)
    n: number of variables
    returns: (solvable: bool, where: list[int], rows: list[int])
             where[col] = pivot row index or -1 if free
             rows is transformed in-place to reduced form
    """
    where = [-1] * n
    r = 0  # current pivot row
    m = len(rows)

    for c in range(n):
        if r >= m:
            break

        # Find pivot row with coefficient 1 in column c
        pivot = -1
        mask = 1 << c
        for i in range(r, m):
            if rows[i] & mask:
                pivot = i
                break
        if pivot == -1:
            continue  # free variable

        # Swap pivot into position r
        rows[r], rows[pivot] = rows[pivot], rows[r]
        where[c] = r

        # Eliminate column c from all other rows (Gauss-Jordan)
        for i in range(m):
            if i != r and (rows[i] & mask):
                rows[i] ^= rows[r]

        r += 1

    # Check inconsistency: 0 coefficients but RHS = 1
    rhs_bit = 1 << n
    for i in range(r, m):
        coeffs = rows[i] & (rhs_bit - 1)  # lower n bits
        rhs = (rows[i] >> n) & 1
        if coeffs == 0 and rhs == 1:
            return False, where, rows

    return True, where, rows


def any_solution(where, rows, n):
    """
    With Gauss-Jordan form and free variables = 0, pivot variable equals RHS of its row.
    """
    x = [0] * n
    for var in range(n):
        pr = where[var]
        if pr != -1:
            x[var] = (rows[pr] >> n) & 1
    return x


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))

    groups = []
    for _ in range(n):
        k = int(next(it))
        cur = []
        for _ in range(k):
            cur.append(int(next(it)) - 1)  # to 0-based
        groups.append(cur)

    initial = [int(next(it)) for _ in range(n)]

    # Build coefficient matrix in bit form: equation per cell c:
    # XOR of x[i] where c in S_i equals initial[c] XOR target
    #
    # We'll first build for each cell c a bitmask of which variables affect it.
    coeff_mask = [0] * n
    for i in range(n):
        for c in groups[i]:
            coeff_mask[c] |= (1 << i)

    for target in (1, 0):
        rows = []
        for c in range(n):
            rhs = initial[c] ^ target
            row = coeff_mask[c] | (rhs << n)
            rows.append(row)

        solvable, where, reduced = gauss_xor(rows, n)
        if not solvable:
            continue

        x = any_solution(where, reduced, n)
        clicks = [str(i + 1) for i, v in enumerate(x) if v == 1]

        sys.stdout.write(str(len(clicks)) + "\n")
        if clicks:
            sys.stdout.write(" ".join(clicks) + "\n")
        else:
            # match the C++ behavior: extra blank line after 0
            sys.stdout.write("\n")
        return

    sys.stdout.write("-1\n")


if __name__ == "__main__":
    main()
