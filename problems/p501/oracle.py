import sys
from functools import lru_cache

# Python port of the same algorithm:
# - split into upper/lower pyramids
# - solve each pyramid with layer DP on carry masks
# n <= 4 so masks up to 16 bits; recursion enumerates row tilings.

def solve_pyramid(rows, n):
    # Precompute first_row[mask] for top row of 4 cells (cycle).
    # mask: 4-bit blocked pattern (1 means blocked/black).
    first_row = [0] * 16
    for blocked in range(16):
        for e in range(16):  # choose which cycle edges (0-1,1-2,2-3,3-0) are used
            covered = blocked
            ok = True
            for j in range(4):
                if (e >> j) & 1:
                    u, v = j, (j + 1) & 3
                    if (covered >> u) & 1 or (covered >> v) & 1:
                        ok = False
                        break
                    covered |= (1 << u) | (1 << v)
            if ok and covered == 15:
                first_row[blocked] += 1

    # Convert each input row string into bitmask of black cells.
    row_mask = [0] * n
    for r in range(n):
        m = 0
        for i, ch in enumerate(rows[r]):
            if ch == '*':
                m |= 1 << i
        row_mask[r] = m

    # Build mapping arrays like the C++ code.
    bit_mapping = [[0] * 32 for _ in range(5)]
    bit_mapping_next = [[-1] * 32 for _ in range(5)]
    bit_mapping_result = [[-1] * 32 for _ in range(5)]

    for r in range(n):
        cur_len = 2 * r + 1
        nxt_len = 2 * r + 3
        shift = 0
        count = 0
        current = 0
        for g in range(4):
            # even positions can connect down
            for i in range(0, cur_len, 2):
                bit_mapping_next[r][current + i] = shift + (i + 1)
                bit_mapping_result[r][current + i] = count
                count += 1
            # carry bits map to odd positions (there are r per face)
            for i in range(r):
                bit_mapping[r][g * r + i] = current + 2 * i + 1
            current += cur_len
            shift += nxt_len

    if n > 0:
        for i in range(32):
            bit_mapping_next[n - 1][i] = 0  # unused on last row

    def carry_to_layer_mask(row, carry_mask, carry_bits_len):
        """Convert compact carry mask into a bitmask over actual row positions."""
        res = 0
        for i in range(carry_bits_len):
            if (carry_mask >> i) & 1:
                res |= 1 << bit_mapping[row][i]
        return res

    # Precompute transitions next_row[r][incoming_mask] -> list(next_mask)
    # for r=1..n-1 (row 0 is handled separately).
    next_row = [None] * n  # next_row[r] is dict/array for that r
    for r in range(1, n):
        prev_bits = 4 * r
        trans = [[] for _ in range(1 << prev_bits)]
        row_len = 4 * (2 * r + 1)

        # Recursively enumerate all completions of row r.
        def enum(mask, idx, next_lvl_black, res):
            if idx == row_len:
                trans_in.append(res)
                return
            if (mask >> idx) & 1:
                enum(mask, idx + 1, next_lvl_black, res)
                return

            # wrap domino (0 with last)
            if idx == 0 and ((mask >> (row_len - 1)) & 1) == 0:
                enum(mask | (1 << (row_len - 1)), idx + 1, next_lvl_black, res)

            # downward domino if allowed and landing cell not black in next row
            br = bit_mapping_result[r][idx]
            if br != -1:
                landing = bit_mapping_next[r][idx]
                if ((next_lvl_black >> landing) & 1) == 0:
                    enum(mask, idx + 1, next_lvl_black, res | (1 << br))

            # horizontal domino to idx+1
            if idx < row_len - 1 and ((mask >> (idx + 1)) & 1) == 0:
                enum(mask, idx + 2, next_lvl_black, res)

        for incoming in range(1 << prev_bits):
            trans_in = []
            cur_mask = carry_to_layer_mask(r, incoming, prev_bits)

            # carried positions cannot be black
            if (cur_mask & row_mask[r]) != 0:
                continue

            start_mask = cur_mask | row_mask[r]
            next_lvl_black = row_mask[r + 1] if r + 1 < n else 0  # forbid downward dominos into black
            enum(start_mask, 0, next_lvl_black, 0)
            trans[incoming] = trans_in

        next_row[r] = trans

    # DP: dp[r][mask] where mask has 4*r bits.
    dp = [None] * (n + 1)
    dp[1] = [0] * (1 << 4)

    # Initialize from first row
    for carry in range(1 << 4):
        if (carry & row_mask[0]) != 0:
            continue
        if n > 1 and (carry_to_layer_mask(1, carry, 4) & row_mask[1]) != 0:
            continue
        dp[1][carry] += first_row[carry | row_mask[0]]

    # Transition rows 1..n-1
    for r in range(1, n):
        prev_bits = 4 * r
        dp[r] = dp[r] if dp[r] is not None else [0] * (1 << prev_bits)
        dp[r + 1] = [0] * (1 << (4 * (r + 1)))
        for mask in range(1 << prev_bits):
            ways = dp[r][mask]
            if ways == 0:
                continue
            for nxt in next_row[r][mask]:
                dp[r + 1][nxt] += ways

    # Result vector size 2^(4n) (equator mask)
    if n == 1:
        # dp[1] already corresponds to equator mask size 4
        res = [0] * (1 << 4)
        res[:] = dp[1][:]
        return res
    else:
        return dp[n]


def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    layers = data[1:]               # 2n strings
    upper = layers[:n]
    lower = list(reversed(layers[n:]))

    up = solve_pyramid(upper, n)
    down = solve_pyramid(lower, n)

    ans = 0
    for a, b in zip(up, down):
        ans += a * b
    print(ans)

if __name__ == "__main__":
    main()
