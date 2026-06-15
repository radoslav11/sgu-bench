import sys

MOD = 2007

def mat_mul(a, b):
    """Multiply two square matrices modulo MOD."""
    n = len(a)
    c = [[0] * n for _ in range(n)]
    for i in range(n):
        ai = a[i]
        for k in range(n):
            if ai[k] == 0:
                continue
            aik = ai[k]
            bk = b[k]
            for j in range(n):
                c[i][j] = (c[i][j] + aik * bk[j]) % MOD
    return c

def mat_pow(m, p):
    """Raise square matrix m to power p modulo MOD using binary exponentiation."""
    n = len(m)

    # Identity matrix
    res = [[0] * n for _ in range(n)]
    for i in range(n):
        res[i][i] = 1

    while p > 0:
        if p & 1:
            res = mat_mul(res, m)
        m = mat_mul(m, m)
        p >>= 1
    return res

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    N = int(data[0])
    K = int(data[1])

    # Impossible to split N distinct albums into K non-empty groups if N < K
    if N < K:
        print(0)
        return

    # Transition matrix for Stirling numbers S(n,k):
    # S(n,p) = S(n-1,p-1) + p*S(n-1,p)
    # State vector is [S(n,0), S(n,1), ..., S(n,K)]^T
    size = K + 1
    T = [[0] * size for _ in range(size)]

    for p in range(size):
        if p > 0:
            T[p][p] = p % MOD      # coefficient of S(n-1,p)
        if p < K:
            T[p + 1][p] = 1        # coefficient of S(n-1,p) flowing to S(n,p+1)

    # Compute T^N. Since v0 = [1,0,0,...]^T, S(N,K) is (T^N)[K][0].
    TN = mat_pow(T, N)
    print(TN[K][0] % MOD)

if __name__ == "__main__":
    solve()
