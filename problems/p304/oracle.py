import sys
input = sys.stdin.readline

def main():
    N, K, P = map(int, input().split())
    B = list(map(int, input().split()))  # anesthesia costs
    gums = [[] for _ in range(K)]        # list of lists for each gum

    # Read each tooth and append to its gum: (cost, index)
    for idx in range(1, N+1):
        a, c = map(int, input().split())
        gums[c-1].append((a, idx))

    # Sort each gum by ascending tooth cost
    for g in range(K):
        gums[g].sort()

    INF = 10**18
    # dp[g][t] = minimal cost to treat t teeth using first g gums
    # choice[g][t] = how many we took from gum g-1 (or -1 if zero)
    dp = [[INF]*(N+1) for _ in range(K+1)]
    choice = [[0]*(N+1) for _ in range(K+1)]
    dp[0][0] = 0

    # DP transitions
    for g in range(K):
        for t in range(N+1):
            if dp[g][t] == INF:
                continue
            base = dp[g][t]
            # Option 1: take 0
            if base < dp[g+1][t]:
                dp[g+1][t] = base
                choice[g+1][t] = -1
            # Option 2: take p >= 1
            cost_acc = B[g]
            max_p = min(len(gums[g]), N - t)
            for p in range(max_p):
                cost_acc += gums[g][p][0]   # add p-th cheapest tooth cost
                nt = t + p + 1
                nc = base + cost_acc
                if nc < dp[g+1][nt]:
                    dp[g+1][nt] = nc
                    choice[g+1][nt] = p + 1

    # Find the max teeth count within P
    best = 0
    for t in range(N+1):
        if dp[K][t] <= P:
            best = t
    print(best)

    # Reconstruct solution
    res = []
    t = best
    for g in range(K, 0, -1):
        took = choice[g][t]
        if took > 0:
            # collect the first 'took' indices from gums[g-1]
            for i in range(took):
                res.append(gums[g-1][i][1])
            t -= took

    res.sort()
    print(*res)

if __name__ == "__main__":
    main()
