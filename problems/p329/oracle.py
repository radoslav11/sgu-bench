import sys
from functools import lru_cache


def solve():
    # Read all integers from standard input.
    data = list(map(int, sys.stdin.read().split()))

    # First integer is the side length of the large triangle.
    n = data[0]

    # Next four integers are counts of triangle kinds 1..4.
    #
    # Internally:
    # cnt[0] = number of triangles with 0 black edges
    # cnt[1] = number of triangles with 1 black edge
    # cnt[2] = number of triangles with 2 black edges
    # cnt[3] = number of triangles with 3 black edges
    cnt = data[1:5]

    # v_edge[r][p] stores the shared edge between cells p and p+1 in row r.
    #
    # N <= 5, so small fixed arrays are enough.
    v_edge = [[0] * 16 for _ in range(8)]

    # h_edge[r][k] stores the shared edge between row r and row r+1.
    #
    # It is the bottom side of upward cell (r, 2k)
    # and the top side of downward cell (r+1, 2k+1).
    h_edge = [[0] * 16 for _ in range(8)]

    # Memoization dictionary.
    #
    # Key:
    #   (r, p, active_mask, cnt0, cnt1, cnt2, cnt3)
    #
    # Value:
    #   number of valid completions from this state.
    memo = {}

    def active_mask(r, p):
        """
        Compute a compact bitmask of already-written edges that can still affect
        future placements.

        Edges that will never be read again are intentionally ignored.
        """

        # Result bitmask.
        mask = 0

        # Current bit position.
        bit = 0

        # Horizontal edges from the previous row that have not yet been consumed.
        #
        # h_edge[r-1][k] is consumed by downward cells in row r.
        # At position p, edges with index smaller than p//2 have already been used.
        if r >= 2:
            for k in range(p // 2, r - 1):
                # Store this edge color as one bit.
                mask |= (h_edge[r - 1][k] & 1) << bit
                bit += 1

        # The immediate left edge of the current cell, if there is a left neighbor.
        if p > 0:
            mask |= (v_edge[r][p - 1] & 1) << bit
            bit += 1

        # Bottom edges of upward cells already placed in the current row.
        #
        # These edges will be read by downward cells in the next row.
        built = (p + 1) // 2

        for k in range(built):
            mask |= (h_edge[r][k] & 1) << bit
            bit += 1

        # Return the active boundary.
        return mask

    def rec(r, p):
        """
        Return the number of valid ways to complete the board starting before
        cell (r, p).
        """

        # If we are past the final row, the whole triangle was filled correctly.
        if r > n:
            return 1

        # Current active boundary.
        mask = active_mask(r, p)

        # Memoization key includes the current position, active boundary,
        # and remaining triangle counts.
        key = (r, p, mask, cnt[0], cnt[1], cnt[2], cnt[3])

        # Reuse the result if this state was already solved.
        if key in memo:
            return memo[key]

        # Determine the next cell in row-major order.
        next_r = r
        next_p = p + 1

        # Row r contains 2r - 1 cells.
        # If we passed the last cell, move to row r+1.
        if next_p >= 2 * r - 1:
            next_r = r + 1
            next_p = 0

        # Even positions are upward-facing triangles.
        is_up = (p % 2 == 0)

        # The left edge is fixed by the previous cell in the same row,
        # unless this is the first cell of the row.
        fixed_left = v_edge[r][p - 1] if p > 0 else -1

        # For downward triangles, the top edge is fixed by the row above.
        # For upward triangles, there is no top-edge constraint.
        if is_up:
            fixed_top = -1
        else:
            fixed_top = h_edge[r - 1][(p - 1) // 2]

        # Total number of completions from this state.
        total = 0

        # Try every triangle kind.
        for kind in range(4):
            # Skip if no triangle of this kind remains.
            if cnt[kind] == 0:
                continue

            # Internal kind equals the required number of black edges.
            num_black = kind

            # Enumerate all 3-bit edge color assignments.
            #
            # bit 0: left edge
            # bit 1: right edge
            # bit 2: third edge
            #
            # For upward triangles, third = bottom.
            # For downward triangles, third = top.
            for orient_mask in range(8):
                # This orientation is only valid if it has the required
                # number of black edges.
                if orient_mask.bit_count() != num_black:
                    continue

                # Extract edge colors.
                e_left = (orient_mask >> 0) & 1
                e_right = (orient_mask >> 1) & 1
                e_third = (orient_mask >> 2) & 1

                # Match the already-fixed left edge, if any.
                if fixed_left != -1 and fixed_left != e_left:
                    continue

                # For downward triangles, the third edge is the top edge,
                # which must match the fixed edge from the row above.
                if not is_up and fixed_top != e_third:
                    continue

                # Write the right edge for the next cell in this row.
                v_edge[r][p] = e_right

                # If upward, write the bottom edge for the next row.
                if is_up:
                    h_edge[r][p // 2] = e_third

                # Use one triangle of this kind.
                cnt[kind] -= 1

                # Count all completions after this placement.
                total += rec(next_r, next_p)

                # Backtrack.
                cnt[kind] += 1

        # Store answer for this DP state.
        memo[key] = total

        # Return computed result.
        return total

    # Start at the first row and first position.
    print(rec(1, 0))


if __name__ == "__main__":
    solve()
