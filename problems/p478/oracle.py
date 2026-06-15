import sys

def solve():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    a = int(next(it))  # total boys
    b = int(next(it))  # total girls
    n = int(next(it))  # number of notes
    rec = [int(next(it)) for _ in range(n)]  # recorded group sizes
    # Prepend the implicit initial "note": starting size is exactly b (0 boys, b girls).
    cnt = [b] + rec

    # dp[pos][boys] is True if we can reach after processing 'pos' notes (pos in [0..n])
    # having exactly 'boys' boys with the teacher.
    dp = [[False] * (a + 1) for _ in range(n + 1)]

    # par[pos][boys] = (prev_boys, boys_joined_this_step, girls_left_this_step)
    # to reconstruct the schedule. Only meaningful when dp[pos][boys] is True and pos > 0.
    par = [[None] * (a + 1) for _ in range(n + 1)]

    # Initial state: before any recorded note, we are at pos = 0 with 0 boys.
    # Girls are implicitly cnt[0] - 0 = b.
    dp[0][0] = True

    # Iterate over positions 0..n-1 and try to reach pos+1.
    for pos in range(n):
        for boys in range(a + 1):
            if not dp[pos][boys]:
                continue
            # Compute current girls; this must match the recorded size at pos.
            girls = cnt[pos] - boys
            if girls < 0 or girls > b:
                # This state cannot be valid; skip.
                continue

            # Try all possibilities for the number of girls leaving before next note.
            for girls_leave in range(girls + 1):
                new_girls = girls - girls_leave
                # Next recorded total fixes the new number of boys.
                new_boys = cnt[pos + 1] - new_girls

                # Validate non-decreasing boys and bounds.
                if new_boys < boys or new_boys < 0 or new_boys > a:
                    continue

                if not dp[pos + 1][new_boys]:
                    dp[pos + 1][new_boys] = True
                    par[pos + 1][new_boys] = (boys, new_boys - boys, girls_leave)

    # Find any reachable end state after n notes.
    end_boys = None
    for c in range(a + 1):
        if dp[n][c]:
            end_boys = c
            break

    if end_boys is None:
        print("ERROR")
        return

    # Reconstruct actions from par.
    actions = []
    pos = n
    boys = end_boys
    while pos > 0:
        prev = par[pos][boys]
        # prev must exist because dp[pos][boys] is True.
        prev_boys, joined, left = prev
        actions.append((joined, left))
        boys = prev_boys
        pos -= 1

    actions.reverse()
    for joined, left in actions:
        print(joined, left)

if __name__ == "__main__":
    solve()
