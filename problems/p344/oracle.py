import sys
from collections import deque

def main():
    input = sys.stdin.readline
    n, m = map(int, input().split())
    grid = [input().rstrip('\n') for _ in range(n)]

    # Flattened indexing: u = i*m + j
    k = n * m
    visited = [False] * k       # True if this cell currently has weed
    degree = [0] * k            # Number of weed neighbors

    q = deque()
    # Enqueue initial weed cells
    for i in range(n):
        for j in range(m):
            if grid[i][j] == 'X':
                u = i * m + j
                visited[u] = True
                q.append(u)

    ans = 0
    # Directions: up, down, left, right
    dirs = [(-1,0),(1,0),(0,-1),(0,1)]

    # BFS-like propagation
    while q:
        u = q.popleft()
        ans += 1
        i, j = divmod(u, m)
        # Update each neighbor
        for di, dj in dirs:
            ni, nj = i + di, j + dj
            if 0 <= ni < n and 0 <= nj < m:
                v = ni * m + nj
                degree[v] += 1
                # If an empty cell now has ≥2 weed neighbors, it becomes weed
                if degree[v] == 2 and not visited[v]:
                    visited[v] = True
                    q.append(v)

    print(ans)

if __name__ == "__main__":
    main()
