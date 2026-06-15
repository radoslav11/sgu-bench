import sys
sys.setrecursionlimit(10**7)

def read_ints():
    return list(map(int, sys.stdin.readline().split()))

def main():
    # Read alphabet and map each char to an index 0..σ-1
    alphabet = sys.stdin.readline().strip()
    sigma = {ch:i for i,ch in enumerate(alphabet)}
    m = len(alphabet)

    # Read number of states K
    K = int(sys.stdin.readline())
    data = read_ints()
    S = data[0] - 1                # initial state (0-based)
    L = data[1]                    # number of terminal states
    terminals = [x-1 for x in data[2:2+L]]

    # Read φ table: K rows of m integers
    phi = [None]*K
    for u in range(K):
        row = read_ints()
        phi[u] = [x-1 for x in row]

    # Read χ table: K rows of m integers
    chi = [None]*K
    for u in range(K):
        chi[u] = read_ints()

    # Read N: desired length of strings
    N = int(sys.stdin.readline())

    # Precompute effective transitions:
    # trans[u][c] = state where c is finally consumed; -1 if infinite loop
    trans = [[None]*m for _ in range(K)]

    def dfs(u, c):
        if chi[u][c] == 0:
            # this edge consumes c immediately
            trans[u][c] = u
            return
        if trans[u][c] is not None:
            # already resolved or in-progress
            return
        trans[u][c] = -1  # mark in-progress to detect cycles
        v = phi[u][c]
        dfs(v, c)
        trans[u][c] = trans[v][c]  # inherit result

    for u in range(K):
        for c in range(m):
            if trans[u][c] is None:
                dfs(u, c)

    # dp[i][u] = number of ways to be in state u after consuming i chars
    dp = [ [0]*K for _ in range(N+1) ]
    dp[0][S] = 1

    for i in range(N):
        for u in range(K):
            ways = dp[i][u]
            if ways == 0:
                continue
            for c in range(m):
                a = trans[u][c]
                if a == -1:
                    # cannot consume c (infinite non-absorbing loop)
                    continue
                v = phi[a][c]     # consuming c from state a
                dp[i+1][v] += ways

    # Sum counts at length N over terminal states
    result = sum(dp[N][t] for t in terminals)
    print(result)

if __name__ == "__main__":
    main()
