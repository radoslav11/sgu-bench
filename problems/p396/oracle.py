import sys

INF = 10**18

def solve() -> None:
    input = sys.stdin.readline
    n = int(input().strip())
    s = input().strip()

    # Map buttons to indices to make DP small and fast
    char_to_idx = {'L': 0, 'U': 1, 'R': 2, 'D': 3}
    idx_to_char = ['L', 'U', 'R', 'D']

    # dp[l][r][last] for current i, and ndp for i+1
    # last: 0 left pressed, 1 right pressed, 2 none, 3 both (jump)
    dp = [[[INF] * 4 for _ in range(4)] for __ in range(4)]
    parent = [[[[None] * 4 for _ in range(4)] for __ in range(4)] for ___ in range(n + 1)]

    # Initial state: left on L(0), right on R(2), last=2 (none)
    dp[0][2][2] = 0

    for i in range(n):
        req_char = s[i]
        req = -1 if req_char == 'N' else char_to_idx[req_char]

        ndp = [[[INF] * 4 for _ in range(4)] for __ in range(4)]

        for l in range(4):
            for r in range(4):
                if l == r:
                    continue
                for last in range(4):
                    cur = dp[l][r][last]
                    if cur >= INF:
                        continue

                    # Helper to relax a transition into ndp
                    def upd(add_cost: int, nl: int, nr: int, nlast: int):
                        new_cost = cur + add_cost
                        if new_cost < ndp[nl][nr][nlast]:
                            ndp[nl][nr][nlast] = new_cost
                            parent[i + 1][nl][nr][nlast] = (l, r, last)

                    # 1) Do nothing: only if no required button
                    if req == -1:
                        upd(0, l, r, 2)

                    # 2) Press with left without moving
                    if req == -1 or l == req:
                        upd(1, l, r, 0)

                    # 3) Press with right without moving
                    if req == -1 or r == req:
                        upd(1, l, r, 1)

                    # 4) Move left to a vacant button and press it
                    for nl in range(4):
                        if nl == r:
                            continue  # must be vacant
                        if nl == 2 and r == 0:
                            continue  # illegal crossed (R,L)
                        if req != -1 and nl != req:
                            continue  # must press required if any

                        move_cost = 9 if (last == 0 or last == 3) else 3
                        upd(move_cost, nl, r, 0)

                    # 5) Move right to a vacant button and press it
                    for nr in range(4):
                        if nr == l:
                            continue
                        if l == 2 and nr == 0:
                            continue  # illegal crossed
                        if req != -1 and nr != req:
                            continue

                        move_cost = 9 if (last == 1 or last == 3) else 3
                        upd(move_cost, l, nr, 1)

                    # 6) Jump: move both, press both
                    for nl in range(4):
                        for nr in range(4):
                            if nl == nr:
                                continue
                            if nl == 2 and nr == 0:
                                continue  # illegal crossed
                            if req != -1 and nl != req and nr != req:
                                continue  # required must be among pressed
                            upd(10, nl, nr, 3)

        dp = ndp

    # Choose best final state
    best = INF
    best_state = None
    for l in range(4):
        for r in range(4):
            for last in range(4):
                if dp[l][r][last] < best:
                    best = dp[l][r][last]
                    best_state = (l, r, last)

    # Reconstruct path
    path = []
    l, r, last = best_state
    for i in range(n, 0, -1):
        path.append((l, r))
        l, r, last = parent[i][l][r][last]

    path.reverse()

    # Output
    out = [str(best)]
    for l, r in path:
        out.append(idx_to_char[l] + idx_to_char[r])
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
