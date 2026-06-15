import sys
sys.setrecursionlimit(10**7)
def input():
    return sys.stdin.readline()

# Hopcroft–Karp for bipartite perfect matching
from collections import deque
class HopcroftKarp:
    def __init__(self,n,m):
        self.n, self.m = n, m
        self.adj = [[] for _ in range(n)]
    def add_edge(self,u,v):
        self.adj[u].append(v)
    def max_matching(self):
        # -1 means unmatched
        matchL = [-1]*self.n
        matchR = [-1]*self.m
        dist = [0]*self.n
        INF = 10**9

        def bfs():
            q=deque()
            for u in range(self.n):
                if matchL[u]==-1:
                    dist[u]=0
                    q.append(u)
                else:
                    dist[u]=INF
            found=False
            while q:
                u=q.popleft()
                for v in self.adj[u]:
                    mu=matchR[v]
                    if mu<0:
                        found=True
                    elif dist[mu]==INF:
                        dist[mu]=dist[u]+1
                        q.append(mu)
            return found

        def dfs(u):
            for v in self.adj[u]:
                mu=matchR[v]
                if mu<0 or (dist[mu]==dist[u]+1 and dfs(mu)):
                    matchL[u]=v
                    matchR[v]=u
                    return True
            dist[u]=INF
            return False

        res=0
        while bfs():
            for u in range(self.n):
                if matchL[u]==-1 and dfs(u):
                    res+=1
        self.matchL, self.matchR = matchL, matchR
        return res

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    K = int(next(it))
    M = N*K//2
    A = [0]*M
    B = [0]*M
    for i in range(M):
        A[i] = int(next(it))-1
        B[i] = int(next(it))-1

    # Build adjacency for undirected graph
    adj = [[] for _ in range(N)]
    for i in range(M):
        u, v = A[i], B[i]
        adj[u].append((v,i))
        adj[v].append((u,i))

    # Prepare to find Euler tours and orient edges
    used = [False]*M
    directed = []  # list of (u,v,index+1)
    stack = []
    # We'll reuse a stack for each component
    for start in range(N):
        if not adj[start]:
            continue
        stack = [start]
        path = []    # will hold the Euler circuit in vertex order
        edge_stack = []  # parallel stack to record which edge we took
        while stack:
            v = stack[-1]
            # remove used edges
            while adj[v] and used[adj[v][-1][1]]:
                adj[v].pop()
            if not adj[v]:
                stack.pop()
                path.append(v)
                if edge_stack:
                    ei, pu = edge_stack.pop()
                    # orient from pu->v
                    directed.append((pu,v,ei+1))
            else:
                u,ei = adj[v].pop()
                if used[ei]:
                    continue
                used[ei] = True
                stack.append(u)
                edge_stack.append((ei,v))

    # Now build bipartite graph for the directed digraph
    hk = HopcroftKarp(N,N)
    # We also need to remember indices for each (u->v)
    edge_map = [[] for _ in range(N)]
    for u,v,ei in directed:
        hk.add_edge(u,v)
        edge_map[u].append((v,ei))

    if hk.max_matching() < N:
        print("NO")
        return
    print("YES")
    # extract one matched edge per u
    res = [0]*N
    for u in range(N):
        v = hk.matchL[u]
        # pick the stored edge-index that goes to v
        for vv,ei in edge_map[u]:
            if vv==v:
                res[u] = ei
                break
    # print each on its own line
    out = "\n".join(str(x) for x in res)
    print(out)

if __name__=="__main__":
    main()
