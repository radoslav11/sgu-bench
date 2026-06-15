import sys

def solve(N, M, A):
    INF = 10**9
    # Build Q: M rows, N+M columns
    Q = [[INF] * (N + M) for _ in range(M)]

    # Map A into Q by diagonal index s = x+y
    for x in range(N):
        for y in range(M):
            s = x + y
            Q[y][s] = A[x][y]

    # DP: compute min over suffix region (down and right moves)
    # iterate rows and cols in reverse
    for i in range(M - 1, -1, -1):
        for j in range(N + M - 1, -1, -1):
            curr = Q[i][j]
            # candidate from cell below
            down = Q[i + 1][j] if i + 1 < M else INF
            # candidate from cell to the right
            right = Q[i][j + 1] if j + 1 < N + M else INF
            # store the minimum of the three
            Q[i][j] = min(curr, down, right)

    # Extract B by reversing the mapping
    B = [[0]*M for _ in range(N)]
    for i in range(N):
        for j in range(M):
            s = i + j
            B[i][j] = Q[j][s]
    return B

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N, M = map(int, (next(it), next(it)))
    A = [[int(next(it)) for _ in range(M)] for _ in range(N)]
    B = solve(N, M, A)
    out = []
    for row in B:
        out.append(" ".join(map(str, row)))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
