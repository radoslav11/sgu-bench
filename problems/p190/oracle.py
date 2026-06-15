import sys
from collections import deque

def hopcroft_karp(adj, n_left, n_right):
    # adj: list of lists, adj[u] = neighbors v in [0..n_right)
    INF = 10**9
    pair_u = [-1]*n_left
    pair_v = [-1]*n_right
    dist = [0]*n_left

    def bfs():
        queue = deque()
        for u in range(n_left):
            if pair_u[u] == -1:
                dist[u] = 0
                queue.append(u)
            else:
                dist[u] = INF
        found_augment = False
        while queue:
            u = queue.popleft()
            for v in adj[u]:
                pu = pair_v[v]
                if pu == -1:
                    found_augment = True
                elif dist[pu] == INF:
                    dist[pu] = dist[u] + 1
                    queue.append(pu)
        return found_augment

    def dfs(u):
        for v in adj[u]:
            pu = pair_v[v]
            if pu == -1 or (dist[pu] == dist[u] + 1 and dfs(pu)):
                pair_u[u] = v
                pair_v[v] = u
                return True
        dist[u] = INF
        return False

    matching = 0
    while bfs():
        for u in range(n_left):
            if pair_u[u] == -1 and dfs(u):
                matching += 1
    return matching, pair_u, pair_v

def main():
    input_data = sys.stdin.read().strip().split()
    it = iter(input_data)
    n, p = map(int, (next(it), next(it)))
    removed = set()
    for _ in range(p):
        x, y = map(int, (next(it), next(it)))
        removed.add((x,y))

    # Build board of free cells
    total = n*n - p
    # If odd free cells, impossible
    if total % 2:
        print("No")
        return

    black, white = [], []
    bid, wid = {}, {}
    for i in range(1, n+1):
        for j in range(1, n+1):
            if (i,j) in removed: continue
            if (i+j) & 1 == 0:
                bid[(i,j)] = len(black)
                black.append((i,j))
            else:
                wid[(i,j)] = len(white)
                white.append((i,j))

    # Build adjacency from blacks to whites
    adj = [[] for _ in range(len(black))]
    dirs = [(-1,0),(1,0),(0,-1),(0,1)]
    for u,(x,y) in enumerate(black):
        for dx,dy in dirs:
            vcoord = (x+dx, y+dy)
            if vcoord in wid:
                adj[u].append(wid[vcoord])

    # Run matching
    match_sz, pair_u, pair_v = hopcroft_karp(adj, len(black), len(white))
    if match_sz*2 != total:
        print("No")
        return

    print("Yes")
    horiz, vert = [], []
    # Each u matched to pair_u[u] ⇒ a domino
    for u,v in enumerate(pair_u):
        if v == -1: continue
        bx, by = black[u]
        wx, wy = white[v]
        if bx == wx:
            # horizontal: choose left cell
            if by < wy: horiz.append((bx,by))
            else:       horiz.append((wx,wy))
        else:
            # vertical: choose bottom cell
            if bx < wx: vert.append((bx,by))
            else:       vert.append((wx,wy))

    # Output vertical then horizontal (vertical count first)
    print(len(vert))
    for x,y in vert: print(x, y)
    print(len(horiz))
    for x,y in horiz: print(x, y)

if __name__ == "__main__":
    main()
