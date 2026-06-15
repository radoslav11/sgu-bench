import sys

INF = 10**30

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    K = int(next(it))
    N = int(next(it))

    green = []  # list of (cost, original_index)
    black = []  # list of (cost, original_index)

    for i in range(1, N + 1):
        c = int(next(it))
        s = int(next(it))
        if s == 0:
            green.append((c, i))
        else:
            black.append((c, i))

    # Sort by increasing cost so that taking a prefix is optimal
    green.sort()
    black.sort()

    ng = len(green)
    nb = len(black)

    # Tail states:
    # 0=start, 1=...G, 2=...B, 3=...GG, 4=...BB
    transition = [
        [1, 2],   # start
        [3, 2],   # ...G
        [1, 4],   # ...B
        [-1, 2],  # ...GG
        [1, -1],  # ...BB
    ]
    NUM_STATES = 5

    max_b = min(nb, K)
    max_g = min(ng, K)

    # dp[cb][cg][s] = min cost, initialize to INF
    dp = [[[INF] * NUM_STATES for _ in range(max_g + 1)] for __ in range(max_b + 1)]

    # parent pointers to reconstruct:
    # par[cb][cg][s] = (prev_cb, prev_cg, prev_state)
    par = [[[(0, 0, 0)] * NUM_STATES for _ in range(max_g + 1)] for __ in range(max_b + 1)]

    dp[0][0][0] = 0

    for cb in range(max_b + 1):
        for cg in range(max_g + 1):
            pos = cb + cg
            if pos >= K:
                continue
            guests = K - pos  # guests in next round

            for s in range(NUM_STATES):
                cur = dp[cb][cg][s]
                if cur >= INF:
                    continue

                # choice=0 -> green, choice=1 -> black
                for choice in (0, 1):
                    ns = transition[s][choice]
                    if ns == -1:
                        continue

                    if choice == 0:
                        cnt = cg
                        if cnt >= ng or cnt >= K or cg >= max_g:
                            continue
                        cost = green[cnt][0]
                        ncb, ncg = cb, cg + 1
                    else:
                        cnt = cb
                        if cnt >= nb or cnt >= K or cb >= max_b:
                            continue
                        cost = black[cnt][0]
                        ncb, ncg = cb + 1, cg

                    new_cost = cur + guests * cost
                    if new_cost < dp[ncb][ncg][ns]:
                        dp[ncb][ncg][ns] = new_cost
                        par[ncb][ncg][ns] = (cb, cg, s)

    # pick best end with cb+cg=K
    best = INF
    best_cb = best_cg = best_s = -1

    for cb in range(max_b + 1):
        cg = K - cb
        if cg < 0 or cg > max_g:
            continue
        for s in range(NUM_STATES):
            if dp[cb][cg][s] < best:
                best = dp[cb][cg][s]
                best_cb, best_cg, best_s = cb, cg, s

    if best >= INF:
        sys.stdout.write("Impossible\n")
        return

    # reconstruct chosen tea indices in order
    ans = [0] * K
    cb, cg, s = best_cb, best_cg, best_s

    for pos in range(K - 1, -1, -1):
        pcb, pcg, ps = par[cb][cg][s]

        # If green count decreased, last pick was green; else black
        if pcg < cg:
            ans[pos] = green[cg - 1][1]
        else:
            ans[pos] = black[cb - 1][1]

        cb, cg, s = pcb, pcg, ps

    sys.stdout.write(" ".join(map(str, ans)) + "\n")


if __name__ == "__main__":
    solve()
