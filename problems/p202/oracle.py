import sys
sys.setrecursionlimit(10**7)

def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    m = int(data[1])

    INF = 10**18

    # dp[i][j] = minimal moves to move i discs using j pegs
    dp = [[INF] * (m + 1) for _ in range(n + 1)]

    # 0 discs: 0 moves regardless of pegs
    for j in range(1, m + 1):
        dp[0][j] = 0

    # 1 disc: 1 move if j >= 2
    for j in range(2, m + 1):
        dp[1][j] = 1

    # Frame–Stewart recurrence:
    # dp[i][j] = min_{1<=k<i} (2*dp[k][j] + dp[i-k][j-1])
    for i in range(2, n + 1):
        for j in range(3, m + 1):
            best = INF
            for k in range(1, i):
                cand = 2 * dp[k][j] + dp[i - k][j - 1]
                if cand < best:
                    best = cand
            dp[i][j] = best

    # Simulate pegs as lists used like stacks (append/pop from end).
    pegs = [[] for _ in range(m + 1)]
    for disc in range(n, 0, -1):
        pegs[1].append(disc)

    out_lines = []
    def move_disc(frm: int, to: int) -> None:
        """Move the top disc from peg frm to peg to, printing with 'atop' if needed."""
        disc = pegs[frm][-1]
        if pegs[to]:
            out_lines.append(f"move {disc} from {frm} to {to} atop {pegs[to][-1]}")
        else:
            out_lines.append(f"move {disc} from {frm} to {to}")
        pegs[frm].pop()
        pegs[to].append(disc)

    def rec(cnt: int, num_pegs: int, frm: int, to: int) -> None:
        """Reconstruct one optimal solution using dp, moving cnt discs from frm to to."""
        if cnt == 0:
            return
        if cnt == 1:
            move_disc(frm, to)
            return

        # Find a split k consistent with dp optimal value.
        for k in range(1, cnt):
            if 2 * dp[k][num_pegs] + dp[cnt - k][num_pegs - 1] == dp[cnt][num_pegs]:
                # Choose an auxiliary peg j not equal to frm or to, where the next disc can be placed.
                top_disc = pegs[frm][-1]
                for j in range(1, m + 1):
                    if j == frm or j == to:
                        continue
                    if (not pegs[j]) or pegs[j][-1] > top_disc:
                        # 1) move k discs to j using all num_pegs pegs
                        rec(k, num_pegs, frm, j)
                        # 2) move remaining cnt-k discs to destination using num_pegs-1 pegs
                        rec(cnt - k, num_pegs - 1, frm, to)
                        # 3) move k discs from j to destination using all num_pegs pegs
                        rec(k, num_pegs, j, to)
                        return

        # If dp is correct, we should never reach here.
        raise RuntimeError("No valid reconstruction split found")

    # Print number of moves, then the moves
    rec(n, m, 1, m)
    sys.stdout.write(str(dp[n][m]) + "\n")
    sys.stdout.write("\n".join(out_lines) + ("\n" if out_lines else ""))

if __name__ == "__main__":
    solve()
