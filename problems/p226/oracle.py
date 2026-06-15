from collections import deque
import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    
    # Build adjacency list: adj[u] = list of (v, color)
    # We'll store colors as 0,1,2 internally
    adj = [[] for _ in range(n)]
    for _ in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        c = int(next(it)) - 1
        adj[u].append((v, c))
    
    INF = 10**9
    # dist[u][c] = shortest #edges to reach u with last edge color c
    dist = [[INF]*3 for _ in range(n)]
    # Starting at vertex 0 with any 'last color'
    for c in range(3):
        dist[0][c] = 0
    
    # BFS queue of (vertex, last_color)
    q = deque()
    for c in range(3):
        q.append((0, c))
    
    while q:
        u, last_color = q.popleft()
        d = dist[u][last_color]
        # Traverse all outgoing edges
        for v, c in adj[u]:
            if c != last_color and dist[v][c] > d + 1:
                dist[v][c] = d + 1
                q.append((v, c))
    
    # Answer is the minimum over last_color at vertex n-1
    ans = min(dist[n-1])
    print(-1 if ans >= INF else ans)

if __name__ == "__main__":
    main()
