import sys

def read_ints():
    return list(map(int, sys.stdin.readline().split()))

def solve():
    # Read N, M, K, L
    n, m, k, l = read_ints()

    # Read M operations (Si, Di); convert Si to 0-based
    ops_raw = read_ints()
    ops = []
    for i in range(m):
        s = ops_raw[2*i] - 1
        d = ops_raw[2*i + 1]
        ops.append((s, d))

    # Read L constraints: each is two K-bit strings (before, after)
    constraints = []
    for _ in range(l):
        sb = sys.stdin.readline().strip()
        sa = sys.stdin.readline().strip()

        before = 0
        after = 0
        # Convert strings into bitmasks, bit i corresponds to position i
        for i, ch in enumerate(sb):
            if ch == '1':
                before |= 1 << i
        for i, ch in enumerate(sa):
            if ch == '1':
                after |= 1 << i

        constraints.append((before, after))

    # Read final N-bit row
    s_final = sys.stdin.readline().strip()
    final_row = 0
    for i, ch in enumerate(s_final):
        if ch == '1':
            final_row |= 1 << i

    # -------------------------
    # 1) Reconstruct vector A (K-1 unknown bits) using Gaussian elimination over GF(2)
    # Each equation:
    #   XOR_{i=0..K-2} (before[i+1] & A[i]) = after[K-1] XOR before[0]
    # Coeff mask is before>>1 (bits 0..K-2)
    # -------------------------
    eqs = []
    for before, after in constraints:
        coeff = before >> 1
        rhs = ((after >> (k - 1)) & 1) ^ (before & 1)
        eqs.append([coeff, rhs])

    # Gaussian elimination in GF(2) for columns 0..K-2
    pivot_row = 0
    for col in range(k - 1):
        if pivot_row >= l:
            break

        # Find pivot row with this column bit set
        found = -1
        for r in range(pivot_row, l):
            if (eqs[r][0] >> col) & 1:
                found = r
                break
        if found == -1:
            continue

        # Swap into pivot position
        eqs[pivot_row], eqs[found] = eqs[found], eqs[pivot_row]

        # Eliminate this column from all other rows
        piv_coeff, piv_rhs = eqs[pivot_row]
        for r in range(l):
            if r != pivot_row and ((eqs[r][0] >> col) & 1):
                eqs[r][0] ^= piv_coeff
                eqs[r][1] ^= piv_rhs

        pivot_row += 1

    # Extract A from reduced system
    a = 0
    for coeff, rhs in eqs:
        if coeff == 0:
            continue
        # index of least significant set bit
        col = (coeff & -coeff).bit_length() - 1
        if rhs:
            a |= 1 << col

    # -------------------------
    # 2) Reverse operations by applying X^{-1} repeatedly to each affected K-block
    # -------------------------
    mask_k = (1 << k) - 1           # mask to keep K bits
    lower_mask = (1 << (k - 1)) - 1 # mask for bits 0..K-2 inside a K-bit block

    row = final_row

    # Process operations in reverse order
    for s, d in reversed(ops):
        for _ in range(d):
            # sub is the current K-bit block (this corresponds to "after" in inverse)
            sub = (row >> s) & mask_k

            # parity of (sub[0..K-2] & a)
            # Python: bit_count() is popcount
            flip = (((sub & a) & lower_mask).bit_count() & 1)

            # Fix top bit to recover the shifted state before undoing rotation
            shifted = sub ^ (flip << (k - 1))

            # Undo the cyclic left shift (rotate right by 1 within K bits)
            original = ((shifted << 1) | (shifted >> (k - 1))) & mask_k

            # Put block back into row
            row = (row & ~(mask_k << s)) | (original << s)

    # Output row as N bits (bit i corresponds to position i)
    out = []
    for i in range(n):
        out.append('1' if ((row >> i) & 1) else '0')
    sys.stdout.write(''.join(out) + "\n")

if __name__ == "__main__":
    solve()
