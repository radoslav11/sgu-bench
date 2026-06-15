import sys

# Very negative value used when no risky move exists.
NEG = -(10 ** 30)

# Directions for chess pieces.
KNIGHT_D = [(-2, -1), (-2, 1), (-1, -2), (-1, 2),
            (1, -2), (1, 2), (2, -1), (2, 1)]

KING_D = [(-1, -1), (-1, 0), (-1, 1),
          (0, -1),           (0, 1),
          (1, -1),  (1, 0),  (1, 1)]

ROOK_D = [(-1, 0), (1, 0), (0, -1), (0, 1)]
BISHOP_D = [(-1, -1), (-1, 1), (1, -1), (1, 1)]


def solve():
    data = sys.stdin.read().split()
    it = iter(data)

    n = int(next(it))
    m = int(next(it))

    bonus_pawn = int(next(it))
    bonus_rook = int(next(it))
    bonus_knight = int(next(it))
    bonus_bishop = int(next(it))
    bonus_queen = int(next(it))
    bonus_king = int(next(it))
    penalty = int(next(it))

    grid = [list(next(it)) for _ in range(n)]

    start_r = start_c = -1

    for r in range(n):
        for c in range(m):
            if grid[r][c] == '@':
                start_r, start_c = r, c

    def in_board(r, c):
        """Return True if cell is inside the board."""
        return 0 <= r < n and 0 <= c < m

    def is_piece(ch):
        """Return True if ch is a capturable stationary chess piece."""
        return ch in "PRKBQM"

    def blocks_ray(ch):
        """
        Return True if ch blocks a sliding attack.

        Walls and pieces block.
        Empty cells and '@' do not block.
        """
        return ch != '.' and ch != '@'

    def capture_bonus(ch):
        """Return score bonus for capturing piece ch."""
        if ch == 'P':
            return bonus_pawn
        if ch == 'R':
            return bonus_rook
        if ch == 'K':
            return bonus_knight
        if ch == 'B':
            return bonus_bishop
        if ch == 'Q':
            return bonus_queen
        if ch == 'M':
            return bonus_king
        return 0

    def for_moves(piece_type, r, c, fn):
        """
        Iterate over legal moves of our selected piece.

        fn(nr, nc) is called for each candidate cell.
        For sliding pieces, if fn returns True, the ray stops.
        """
        if piece_type == 'K':
            dirs = KNIGHT_D
            slide = False
        elif piece_type == 'M':
            dirs = KING_D
            slide = False
        elif piece_type == 'R':
            dirs = ROOK_D
            slide = True
        else:
            dirs = BISHOP_D
            slide = True

        if not slide:
            for dr, dc in dirs:
                nr = r + dr
                nc = c + dc

                if in_board(nr, nc):
                    fn(nr, nc)
        else:
            for dr, dc in dirs:
                nr = r + dr
                nc = c + dc

                while in_board(nr, nc):
                    stop = fn(nr, nc)

                    if stop:
                        break

                    nr += dr
                    nc += dc

    def for_footprint(g, t, pr, pc, fn):
        """
        Iterate over cells attacked by stationary piece t at (pr, pc).
        """

        def ray(dr, dc):
            r = pr + dr
            c = pc + dc

            while in_board(r, c):
                fn(r * m + c)

                if blocks_ray(g[r][c]):
                    break

                r += dr
                c += dc

        def spot(dr, dc):
            r = pr + dr
            c = pc + dc

            if in_board(r, c):
                fn(r * m + c)

        if t == 'R' or t == 'Q':
            for dr, dc in ROOK_D:
                ray(dr, dc)

        if t == 'B' or t == 'Q':
            for dr, dc in BISHOP_D:
                ray(dr, dc)

        if t == 'K':
            for dr, dc in KNIGHT_D:
                spot(dr, dc)

        elif t == 'M':
            for dr, dc in KING_D:
                spot(dr, dc)

        elif t == 'P':
            # Colorless pawns attack toward decreasing row index.
            spot(-1, -1)
            spot(-1, 1)

    def compute_base_count():
        """
        Compute initial number of attacks on each cell.
        """
        cnt = [0] * (n * m)
        active = [0] * (n * m)

        def sweep(dr, dc, diagonal):
            """
            Sweep in one direction and count slider attacks.
            """
            r_range = range(n) if dr >= 0 else range(n - 1, -1, -1)
            c_range = range(m) if dc >= 0 else range(m - 1, -1, -1)

            for r in r_range:
                for c in c_range:
                    pr = r - dr
                    pc = c - dc

                    if not in_board(pr, pc):
                        inc = 0
                    elif blocks_ray(grid[pr][pc]):
                        p = grid[pr][pc]

                        if diagonal:
                            inc = 1 if p == 'B' or p == 'Q' else 0
                        else:
                            inc = 1 if p == 'R' or p == 'Q' else 0
                    else:
                        inc = active[pr * m + pc]

                    active[r * m + c] = inc

                    if inc:
                        cnt[r * m + c] += 1

        # Rook-like directions.
        sweep(0, 1, False)
        sweep(0, -1, False)
        sweep(1, 0, False)
        sweep(-1, 0, False)

        # Bishop-like directions.
        sweep(1, 1, True)
        sweep(1, -1, True)
        sweep(-1, 1, True)
        sweep(-1, -1, True)

        # Stamp pawn, knight, and king attacks directly.
        for r in range(n):
            for c in range(m):
                ch = grid[r][c]

                if is_piece(ch) and ch not in "RBQ":
                    for_footprint(grid, ch, r, c,
                                  lambda idx: cnt.__setitem__(idx, cnt[idx] + 1))

        return cnt

    base_count = compute_base_count()

    def uncount_piece(g, cnt, pr, pc):
        """
        Remove piece at (pr, pc) and subtract its attacks.
        """
        t = g[pr][pc]

        def dec(idx):
            cnt[idx] -= 1

        for_footprint(g, t, pr, pc, dec)

        g[pr][pc] = '.'

    def closure(piece_type, given_g, given_cnt, seeds):
        """
        Compute all free captures from safe seed cells.

        Returns:
            free_sum: total free capture score
            best_risky: best optional final risky capture value
        """
        g = [row[:] for row in given_g]
        cnt = given_cnt[:]

        free_sum = 0

        reach = [0] * (n * m)
        sched = [0] * (n * m)
        risky_mark = [0] * (n * m)

        work = []
        capq = []
        risky_cells = []

        def can_pull(x):
            """
            Return True if x can reach any already reached safe cell.
            """
            xr = x // m
            xc = x % m
            found = False

            def cb(nr, nc):
                nonlocal found

                idx = nr * m + nc

                if reach[idx]:
                    found = True
                    return True

                return g[nr][nc] == '#' or is_piece(g[nr][nc])

            for_moves(piece_type, xr, xc, cb)

            return found

        def on_zero(x):
            """
            Called when attack count of cell x becomes zero.
            """
            r = x // m
            c = x % m

            if g[r][c] == '#' or reach[x]:
                return

            if is_piece(g[r][c]):
                if not sched[x] and can_pull(x):
                    sched[x] = 1
                    capq.append(x)
            else:
                if can_pull(x):
                    reach[x] = 1
                    work.append(x)

        def take(pr, pc):
            """
            Safely capture piece at (pr, pc).
            """
            nonlocal free_sum

            t = g[pr][pc]
            idx = pr * m + pc

            free_sum += capture_bonus(t)

            reach[idx] = 1
            g[pr][pc] = '.'
            work.append(idx)

            def dec(x):
                cnt[x] -= 1

                if cnt[x] == 0:
                    on_zero(x)

            for_footprint(g, t, pr, pc, dec)

        def scan(u):
            """
            Scan all moves from reached cell u.
            """
            ur = u // m
            uc = u % m

            def cb(nr, nc):
                idx = nr * m + nc
                ch = g[nr][nc]

                if ch == '#':
                    return True

                if is_piece(ch):
                    if cnt[idx] == 0:
                        if not sched[idx]:
                            sched[idx] = 1
                            capq.append(idx)
                    else:
                        if not risky_mark[idx]:
                            risky_mark[idx] = 1
                            risky_cells.append(idx)

                    return True

                if cnt[idx] == 0:
                    if not reach[idx]:
                        reach[idx] = 1
                        work.append(idx)

                    return True

                # Attacked empty square cannot be landed on,
                # but a rook/bishop may slide through it.
                return False

            for_moves(piece_type, ur, uc, cb)

        # Insert initial seeds.
        for sr, sc in seeds:
            idx = sr * m + sc

            if not reach[idx]:
                reach[idx] = 1
                work.append(idx)

        # Process until no new reachable cell or capture remains.
        while work or capq:
            while work:
                u = work.pop()
                scan(u)

            while capq:
                p = capq.pop()
                take(p // m, p % m)

        best_risky = NEG

        for idx in risky_cells:
            r = idx // m
            c = idx % m
            ch = g[r][c]

            if is_piece(ch):
                best_risky = max(best_risky, capture_bonus(ch) - penalty)

        return free_sum, best_risky

    def closure_value(piece_type, g, cnt, seeds):
        """
        Free closure value plus optional profitable risky capture.
        """
        free_score, risky = closure(piece_type, g, cnt, seeds)

        if risky > NEG:
            return free_score + max(0, risky)

        return free_score

    def flood_component(piece_type, sr, sc, seen):
        """
        Flood a connected component of currently safe empty cells.
        Used to avoid running closure multiple times for equivalent first moves.
        """
        stack = [(sr, sc)]
        seen[sr * m + sc] = 1

        while stack:
            r, c = stack.pop()

            def cb(nr, nc):
                idx = nr * m + nc
                ch = grid[nr][nc]

                if ch == '#' or is_piece(ch):
                    return True

                if base_count[idx] == 0:
                    if not seen[idx]:
                        seen[idx] = 1
                        stack.append((nr, nc))

                    return True

                return False

            for_moves(piece_type, r, c, cb)

    def eval_reversible(piece_type):
        """
        Evaluate best score if we choose piece_type.
        """
        start_idx = start_r * m + start_c

        # If start is safe, one closure is enough.
        if base_count[start_idx] == 0:
            return closure_value(piece_type, grid, base_count, [(start_r, start_c)])

        best = 0

        empties = []
        capfirst = []

        # Otherwise enumerate possible first moves from attacked start.
        def first_cb(nr, nc):
            nonlocal best

            idx = nr * m + nc
            ch = grid[nr][nc]

            if ch == '#':
                return True

            if is_piece(ch):
                if base_count[idx] == 0:
                    capfirst.append((nr, nc))
                else:
                    best = max(best, capture_bonus(ch) - penalty)

                return True

            if base_count[idx] == 0:
                empties.append((nr, nc))

            return False

        for_moves(piece_type, start_r, start_c, first_cb)

        # Try safe empty first moves, one per component.
        seen = [0] * (n * m)

        for r, c in empties:
            idx = r * m + c

            if not seen[idx]:
                flood_component(piece_type, r, c, seen)
                best = max(best, closure_value(piece_type, grid, base_count, [(r, c)]))

        # Try first move capturing a safe piece.
        for pr, pc in capfirst:
            g2 = [row[:] for row in grid]
            cnt2 = base_count[:]

            first_bonus = capture_bonus(grid[pr][pc])

            uncount_piece(g2, cnt2, pr, pc)

            free_score, risky = closure(piece_type, g2, cnt2, [(pr, pc)])

            value = first_bonus + free_score

            if risky > NEG:
                value += max(0, risky)

            best = max(best, value)

        return best

    answer = 0

    # Queen is forbidden; pawn is dominated by king.
    for piece_type in ['R', 'K', 'B', 'M']:
        answer = max(answer, eval_reversible(piece_type))

    print(answer)


if __name__ == "__main__":
    solve()
