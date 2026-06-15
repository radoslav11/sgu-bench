import sys
sys.setrecursionlimit(10**7)

def solve():
    input = sys.stdin.readline

    n, s, t = map(int, input().split())
    s -= 1
    t -= 1

    grid = [input().strip() for _ in range(n)]
    pop = list(map(int, input().split()))

    # Build adjacency bitmasks: adj[i] has bit j if edge i-j exists
    adj = [0] * n
    for i in range(n):
        row = grid[i]
        m = 0
        for j, ch in enumerate(row):
            if ch == '1':
                m |= 1 << j
        adj[i] = m

    full_mask = (1 << n) - 1
    total_masks = 1 << n

    # total_pop[mask] = sum of populations in 'mask'
    total_pop = [0] * total_masks
    for m in range(1, total_masks):
        lo_bit = m & -m
        i = (lo_bit.bit_length() - 1)
        total_pop[m] = total_pop[m ^ lo_bit] + pop[i]

    # neigh_mask[mask] = union of neighbors of nodes in mask, excluding mask itself
    neigh_mask = [0] * total_masks
    for m in range(total_masks):
        nb = 0
        x = m
        while x:
            lo_bit = x & -x
            i = lo_bit.bit_length() - 1
            x ^= lo_bit
            nb |= adj[i]
        neigh_mask[m] = nb & ~m

    # pow3[i] = 3^i, and pow3_subset[mask] = sum(3^i) for i in mask
    pow3 = [1] * (n + 1)
    for i in range(1, n + 1):
        pow3[i] = pow3[i - 1] * 3

    pow3_subset = [0] * total_masks
    for m in range(1, total_masks):
        lo_bit = m & -m
        i = lo_bit.bit_length() - 1
        pow3_subset[m] = pow3_subset[m ^ lo_bit] + pow3[i]

    # DP tables indexed by ternary-encoded state.
    # dpA[idx] stores value when it's A to move; dpB[idx] for B to move.
    SENT_A = -(1 << 60)
    SENT_B = (1 << 60)
    dpA = [SENT_A] * pow3[n]
    dpB = [SENT_B] * pow3[n]

    def rec(A_mask: int, B_mask: int, idx: int, who: int) -> int:
        """
        Returns optimal final score pop(A) - pop(B) from state (A_mask, B_mask, who).
        idx is the ternary encoding for memoization.
        who: 0 -> A to move (maximize), 1 -> B to move (minimize).
        """
        if who == 0:
            if dpA[idx] != SENT_A:
                return dpA[idx]
        else:
            if dpB[idx] != SENT_B:
                return dpB[idx]

        free = full_mask ^ A_mask ^ B_mask
        terminal = total_pop[A_mask] - total_pop[B_mask]

        # If no free cities remain, no moves can be made.
        if free == 0:
            if who == 0:
                dpA[idx] = terminal
            else:
                dpB[idx] = terminal
            return terminal

        found_self = False   # current player has at least one legal move
        found_other = False  # opponent has at least one legal move (used for pass logic)

        best = SENT_A if who == 0 else SENT_B

        # Enumerate all non-empty submasks of free
        c = free
        while c:
            # Precompute populations needed for the inequalities
            pa = total_pop[neigh_mask[c] & A_mask]
            pb = total_pop[neigh_mask[c] & B_mask]
            pc = total_pop[c]

            a_valid = pa > pc + pb
            b_valid = pb > pc + pa

            if who == 0:
                # A's turn: maximize over all a_valid subsets
                if a_valid:
                    found_self = True
                    val = rec(A_mask | c, B_mask, idx + pow3_subset[c], 1)
                    if val > best:
                        best = val
                if b_valid:
                    found_other = True
            else:
                # B's turn: minimize over all b_valid subsets
                if b_valid:
                    found_self = True
                    val = rec(A_mask, B_mask | c, idx + 2 * pow3_subset[c], 0)
                    if val < best:
                        best = val
                if a_valid:
                    found_other = True

            c = (c - 1) & free

        # If current player can move, commit best minimax value.
        if found_self:
            if who == 0:
                dpA[idx] = best
            else:
                dpB[idx] = best
            return best

        # If neither can move, end game now.
        if not found_other:
            if who == 0:
                dpA[idx] = terminal
            else:
                dpB[idx] = terminal
            return terminal

        # Otherwise current player must pass.
        val = rec(A_mask, B_mask, idx, who ^ 1)
        if who == 0:
            dpA[idx] = val
        else:
            dpB[idx] = val
        return val

    # Initial state: A owns s, B owns t, and A moves first.
    A0 = 1 << s
    B0 = 1 << t
    idx0 = pow3[s] + 2 * pow3[t]
    ans = rec(A0, B0, idx0, 0)
    print(ans)

if __name__ == "__main__":
    solve()
