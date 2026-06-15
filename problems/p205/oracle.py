import sys

INF = 10**18

def solve() -> None:
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)

    n = next(it)
    x = [next(it) for _ in range(n)]

    m = next(it)
    s = next(it)

    # Read level sets L[set][k]
    L = [[next(it) for _ in range(s)] for _ in range(m)]

    # dp_next[set] will represent dp[j+1][set] while iterating backwards
    dp_next = [0] * m

    # To reconstruct, we need the chosen k for each (j, set)
    # choice[j][set] = best k at that state
    choice = [[0] * m for _ in range(n)]

    # Iterate j from n-1 down to 0
    for j in range(n - 1, -1, -1):
        dp_cur = [INF] * m

        for set_idx in range(m):
            best_cost = INF
            best_k = 0

            levels = L[set_idx]
            xj = x[j]

            # Try all k in this set
            for k in range(s):
                next_set = k & (m - 1)
                cost = abs(xj - levels[k]) + dp_next[next_set]
                if cost < best_cost:
                    best_cost = cost
                    best_k = k

            dp_cur[set_idx] = best_cost
            choice[j][set_idx] = best_k

        dp_next = dp_cur

    # dp[0][0] is now dp_next[0] after the loop
    out = []
    out.append(str(dp_next[0]))

    # Reconstruct k's
    cur_set = 0
    ks = []
    for j in range(n):
        k = choice[j][cur_set]
        ks.append(k)
        cur_set = k & (m - 1)

    out.append(" ".join(map(str, ks)))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
