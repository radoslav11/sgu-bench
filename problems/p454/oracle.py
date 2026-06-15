import sys


def solve():
    # Read all input tokens.
    data = sys.stdin.read().strip().split()

    # First two tokens are n and m.
    n = int(data[0])
    m = int(data[1])

    # Remaining tokens describe the grid cells.
    tokens = data[2:]

    # Build the n x m grid of strings.
    g = []
    ptr = 0
    for _ in range(n):
        row = tokens[ptr:ptr + m]
        ptr += m
        g.append(row)

    # h_run[i][j] = horizontal run id of white cell (i, j), or -1.
    h_run = [[-1] * m for _ in range(n)]

    # v_run[i][j] = vertical run id of white cell (i, j), or -1.
    v_run = [[-1] * m for _ in range(n)]

    # Clue sums for horizontal and vertical runs.
    h_clue = []
    v_clue = []

    # Lengths of horizontal and vertical runs.
    h_count = []
    v_count = []

    # Scan all grid cells.
    for i in range(n):
        for j in range(m):
            s = g[i][j]

            # White cells and clue-less black cells do not start runs.
            if s[0] == "." or s == "XXXXX":
                continue

            # AA is the vertical clue part.
            aa = s[:2]

            # BB is the horizontal clue part.
            # s[2] is the backslash.
            bb = s[3:5]

            # Horizontal clue: run goes to the right.
            if bb != "XX":
                rid = len(h_clue)
                h_clue.append(int(bb))

                cnt = 0
                k = j + 1

                # Assign all consecutive white cells to this horizontal run.
                while k < m and g[i][k][0] == ".":
                    h_run[i][k] = rid
                    cnt += 1
                    k += 1

                h_count.append(cnt)

            # Vertical clue: run goes downward.
            if aa != "XX":
                rid = len(v_clue)
                v_clue.append(int(aa))

                cnt = 0
                k = i + 1

                # Assign all consecutive white cells to this vertical run.
                while k < n and g[k][j][0] == ".":
                    v_run[k][j] = rid
                    cnt += 1
                    k += 1

                v_count.append(cnt)

    # kak[length][sum] will contain all masks of distinct digits
    # with exactly "length" digits and total digit sum "sum".
    kak = [[[] for _ in range(46)] for _ in range(10)]

    # Digits are represented by bits 1..9.
    # Bit 0 is unused.
    for mask in range(1 << 10):
        # Ignore digit 0.
        if mask & 1:
            continue

        # Count digits in mask.
        cnt = mask.bit_count()

        # Compute sum of selected digits.
        total_sum = 0
        for d in range(1, 10):
            if mask & (1 << d):
                total_sum += d

        # Maximum possible digit sum is 45.
        if cnt <= 9 and total_sum <= 45:
            kak[cnt][total_sum].append(mask)

    # Number of runs.
    H = len(h_clue)
    V = len(v_clue)

    # Valid full masks for each actual horizontal run.
    h_subs = []
    for r in range(H):
        h_subs.append(kak[h_count[r]][h_clue[r]])

    # Valid full masks for each actual vertical run.
    v_subs = []
    for r in range(V):
        v_subs.append(kak[v_count[r]][v_clue[r]])

    # Collect all white cells.
    cells = []
    for i in range(n):
        for j in range(m):
            if g[i][j][0] == ".":
                cells.append((i, j))

    total_cells = len(cells)

    # Used digit masks for each run.
    h_used = [0] * H
    v_used = [0] * V

    # Whether each white cell has been filled.
    filled = [False] * total_cells

    # Answer grid. Only white cell entries matter.
    ans = [[0] * m for _ in range(n)]

    def allowed_for(subs, used):
        """
        Given:
        - subs: list of valid full masks for a run;
        - used: currently placed digits in that run.

        Return a bitmask of digits that can still be added.
        """
        allowed = 0

        # A full mask remains possible only if it contains all used digits.
        for t in subs:
            if (t & used) == used:
                allowed |= t

        # Already-used digits cannot be reused.
        return allowed & ~used

    def backtrack(placed):
        """
        Recursive search.
        placed = number of already-filled white cells.
        """
        # All white cells filled means success.
        if placed == total_cells:
            return True

        # Compute currently allowed extra digits for every horizontal run.
        h_allowed = [allowed_for(h_subs[r], h_used[r]) for r in range(H)]

        # Compute currently allowed extra digits for every vertical run.
        v_allowed = [allowed_for(v_subs[r], v_used[r]) for r in range(V)]

        # MRV choice: pick unfilled cell with fewest candidates.
        best = -1
        best_mask = 0
        best_cnt = 100

        for idx, (i, j) in enumerate(cells):
            if filled[idx]:
                continue

            # Candidate digits must work in both associated runs.
            cand = h_allowed[h_run[i][j]] & v_allowed[v_run[i][j]]

            # Number of possible digits.
            cnt = cand.bit_count()

            # Update best if this cell is more constrained.
            if cnt < best_cnt:
                best_cnt = cnt
                best = idx
                best_mask = cand

                # Cannot get better than 0 or 1.
                if cnt <= 1:
                    break

        # No candidate for some cell means dead end.
        if best_cnt == 0:
            return False

        # Chosen cell coordinates.
        i, j = cells[best]

        # Run IDs of chosen cell.
        hr = h_run[i][j]
        vr = v_run[i][j]

        # Mark cell filled.
        filled[best] = True

        # Try all digits in candidate mask.
        mask = best_mask
        while mask:
            # Extract lowest set bit.
            bit = mask & -mask

            # Convert bit to digit.
            d = bit.bit_length() - 1

            # Place digit in both run masks.
            h_used[hr] |= bit
            v_used[vr] |= bit

            # Store answer digit.
            ans[i][j] = d

            # Recurse.
            if backtrack(placed + 1):
                return True

            # Undo placement.
            h_used[hr] ^= bit
            v_used[vr] ^= bit

            # Remove this bit from iteration mask.
            mask &= mask - 1

        # Unmark chosen cell before returning failure.
        filled[best] = False

        return False

    # Puzzle is guaranteed solvable.
    backtrack(0)

    # Build output lines.
    output = []

    for i in range(n):
        row = []
        for j in range(m):
            if g[i][j][0] == ".":
                row.append(str(ans[i][j]))
            else:
                row.append("_")
        output.append(" ".join(row))

    print("\n".join(output))


if __name__ == "__main__":
    solve()
