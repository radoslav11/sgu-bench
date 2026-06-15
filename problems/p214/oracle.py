import sys
def read_tokens():
    return sys.stdin.read().split()

def main():
    tokens = read_tokens()
    it = iter(tokens)
    # 1) Read alphabet, strings
    sigma = next(it)
    lam   = next(it)
    mu    = next(it)
    K = len(sigma)
    n = len(lam)
    m = len(mu)

    # 2) Read cost matrix
    cost = [[0]*K for _ in range(K)]
    for i in range(K):
        for j in range(K):
            cost[i][j] = int(next(it))

    # 3) Map each character to its index in sigma
    sigma_idx = {c:i for i,c in enumerate(sigma)}

    # 4) Precompute best matches
    best_for_a = [0]*K
    best_for_b = [0]*K
    for a in range(K):
        # find b minimizing cost[a][b]
        best_for_a[a] = min(range(K), key=lambda b: cost[a][b])
    for b in range(K):
        # find a minimizing cost[a][b]
        best_for_b[b] = min(range(K), key=lambda a: cost[a][b])

    INF = 10**18
    # 5) Initialize DP and back-pointer tables
    dp = [ [INF]*(m+1) for _ in range(n+1) ]
    move = [ [0]   *(m+1) for _ in range(n+1) ]
    dp[0][0] = 0

    # 6) Fill DP
    for i in range(n+1):
        for j in range(m+1):
            cur = dp[i][j]
            if cur == INF:
                continue
            # Option 1: match lam[i] with mu[j]
            if i < n and j < m:
                ai = sigma_idx[lam[i]]
                bi = sigma_idx[mu[j]]
                val = cur + cost[ai][bi]
                if val < dp[i+1][j+1]:
                    dp[i+1][j+1] = val
                    move[i+1][j+1] = 0
            # Option 2: advance in lam only
            if i < n:
                ai = sigma_idx[lam[i]]
                bi = best_for_a[ai]
                val = cur + cost[ai][bi]
                if val < dp[i+1][j]:
                    dp[i+1][j] = val
                    move[i+1][j] = 1
            # Option 3: advance in mu only
            if j < m:
                bi = sigma_idx[mu[j]]
                ai = best_for_b[bi]
                val = cur + cost[ai][bi]
                if val < dp[i][j+1]:
                    dp[i][j+1] = val
                    move[i][j+1] = 2

    # 7) Reconstruct alpha, beta by walking back from (n, m)
    i, j = n, m
    alpha = []
    beta  = []
    while i>0 or j>0:
        mv = move[i][j]
        if mv == 0:
            # matched both characters
            alpha.append(lam[i-1])
            beta .append(mu[j-1])
            i -= 1; j -= 1
        elif mv == 1:
            # advanced in lam only
            ai = sigma_idx[lam[i-1]]
            bi = best_for_a[ai]
            alpha.append(lam[i-1])
            beta .append(sigma[bi])
            i -= 1
        else:
            # advanced in mu only
            bi = sigma_idx[mu[j-1]]
            ai = best_for_b[bi]
            alpha.append(sigma[ai])
            beta .append(mu[j-1])
            j -= 1

    # Reverse because we built them backwards
    alpha.reverse()
    beta .reverse()

    # 8) Print results
    out = []
    out.append(str(dp[n][m]))
    out.append(''.join(alpha))
    out.append(''.join(beta))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
