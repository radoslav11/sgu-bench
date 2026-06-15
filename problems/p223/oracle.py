import sys


def count_placements(n, k):
    full_mask_size = 1 << (n + 1)
    full_mask = full_mask_size - 1

    # dp0[mask][col][rem] = ways for the current row parity.
    dp0 = [[[0] * (k + 1) for _ in range(n + 1)] for __ in range(full_mask_size)]
    dp1 = [[[0] * (k + 1) for _ in range(n + 1)] for __ in range(full_mask_size)]

    dp0[0][0][k] = 1

    for i in range(n):
        for m in range(full_mask_size):
            for c in range(n + 1):
                for r in range(k + 1):
                    dp1[m][c][r] = 0

        # Process all states column by column (matches the C++ reference).
        for col in range(n + 1):
            for mask in range(full_mask_size):
                for rem in range(k + 1):
                    ways = dp0[mask][col][rem]
                    if ways == 0:
                        continue

                    if col == n:
                        dp1[mask][0][rem] += ways
                        continue

                    new_mask = (mask << 1) & full_mask
                    dp0[new_mask][col + 1][rem] += ways

                    bit_left = mask & 1
                    bit_up = (mask >> (n - 1)) & 1
                    bit_dl = (mask >> n) & 1
                    bit_dr = (mask >> (n - 2)) & 1 if n >= 2 else 0

                    if i == 0:
                        bit_up = bit_dl = bit_dr = 0
                    if col == 0:
                        bit_left = bit_dl = 0
                    if col == n - 1:
                        bit_dr = 0

                    if rem > 0 and bit_left == 0 and bit_up == 0 \
                            and bit_dl == 0 and bit_dr == 0:
                        new_mask2 = ((mask << 1) | 1) & full_mask
                        dp0[new_mask2][col + 1][rem - 1] += ways

        dp0, dp1 = dp1, dp0

    return sum(dp0[mask][0][0] for mask in range(full_mask_size))


if __name__ == "__main__":
    data = sys.stdin.read().split()
    n, k = map(int, data)
    print(count_placements(n, k))
