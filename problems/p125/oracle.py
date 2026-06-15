import sys
sys.setrecursionlimit(10000)

def read_input():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    B = []
    idx = 1
    for _ in range(n):
        row = list(map(int, data[idx:idx+n]))
        idx += n
        B.append(row)
    return n, B

def solve():
    n, B = read_input()
    # A: grid we build; b2: working neighbor counts
    A = [[0]*n for _ in range(n)]
    b2 = [row[:] for row in B]

    # Update b2 when placing/removing A[i][j]
    # delta = -1 to place, +1 to remove
    def update(i, j, delta):
        # Returns False if any count goes negative
        ok = True
        for di, dj in [(-1,0),(0,-1)]:
            x, y = i+di, j+dj
            if 0 <= x < n and 0 <= y < n:
                # If neighbor > this, adjust this cell's count
                if A[x][y] > A[i][j]:
                    b2[i][j] += delta
                # If this > neighbor, adjust neighbor's count
                if A[i][j] > A[x][y]:
                    b2[x][y] += delta
                if b2[i][j] < 0 or b2[x][y] < 0:
                    ok = False
        return ok

    # DFS over flat positions
    def dfs(pos):
        if pos == n*n:
            # Check last row all zero
            if all(b2[n-1][j]==0 for j in range(n)):
                for row in A:
                    print(" ".join(map(str,row)))
                sys.exit(0)
            return

        i, j = divmod(pos, n)
        for val in range(10):
            A[i][j] = val
            if update(i, j, -1):
                # Ensure that the above cell (if exists) is settled
                if i==0 or b2[i-1][j]==0:
                    dfs(pos+1)
            update(i, j, +1)

    dfs(0)
    print("NO SOLUTION")

if __name__ == "__main__":
    solve()
