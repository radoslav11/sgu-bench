import sys
sys.setrecursionlimit(10**7)

def read():
    H,W = map(int, sys.stdin.readline().split())
    B = [list(map(int, list(sys.stdin.readline().strip())))
         for _ in range(H-1)]
    return H,W,B

# A simple 2-SAT using Kosaraju
class TwoSat:
    def __init__(self,n):
        self.n = n
        self.adj = [[] for _ in range(2*n)]
        self.radj = [[] for _ in range(2*n)]
    def add_imp(self, x, f, y, g):
        # (x is f) => (y is g)
        u = 2*x + (0 if f else 1)
        v = 2*y + (0 if g else 1)
        self.adj[u].append(v)
        self.radj[v].append(u)
    def add_or(self, x, f, y, g):
        # (x=f) or (y=g)
        # ≡ (¬(x=f) => y=g) and (¬(y=g) => x=f)
        self.add_imp(x, not f, y, g)
        self.add_imp(y, not g, x, f)
    def solve(self):
        n2 = 2*self.n
        used = [False]*n2
        order=[]
        def dfs1(u):
            used[u]=True
            for v in self.adj[u]:
                if not used[v]: dfs1(v)
            order.append(u)
        for i in range(n2):
            if not used[i]: dfs1(i)
        comp=[-1]*n2
        cid=0
        def dfs2(u):
            comp[u]=cid
            for v in self.radj[u]:
                if comp[v]<0: dfs2(v)
        for u in reversed(order):
            if comp[u]<0:
                dfs2(u)
                cid+=1
        assign=[False]*self.n
        for i in range(self.n):
            if comp[2*i]==comp[2*i+1]:
                return None
            assign[i] = (comp[2*i] > comp[2*i+1])
        return assign

def solve():
    H,W,B = read()
    # Try A[0][0] = 0 or 1
    for start in (0,1):
        # build delta table by recurrence
        D = [[0]*W for _ in range(H)]
        D[0][0] = start
        for i in range(1,H):
            for j in range(1,W):
                D[i][j] = B[i-1][j-1] - D[i-1][j] - D[i][j-1] - D[i-1][j-1]

        vars = (H-1)+(W-1)
        ts = TwoSat(vars)

        # impose that for each cell A[i][j] computed must be 0 or 1
        for i in range(1,H):
            for j in range(1,W):
                for ci in (0,1):
                    for rj in (0,1):
                        val = D[i][j]
                        # row var index = H-1 + (j-1)
                        # col var index = i-1
                        val += (ci if j%2==0 else -ci)
                        val += (rj if i%2==0 else -rj)
                        if val not in (0,1):
                            idx_c = i-1
                            idx_r = (H-1)+(j-1)
                            # forbid (c[i]=ci AND r[j]=rj)
                            ts.add_or(idx_c, ci^1, idx_r, rj^1)
        res = ts.solve()
        if res is None:
            continue
        # reconstruct A
        A = [[0]*W for _ in range(H)]
        A[0][0] = start
        for i in range(1,H):
            A[i][0] = res[i-1]
        for j in range(1,W):
            A[0][j] = res[(H-1)+(j-1)]
        for i in range(1,H):
            for j in range(1,W):
                A[i][j] = B[i-1][j-1] - A[i-1][j] - A[i][j-1] - A[i-1][j-1]
        # output
        for row in A:
            print("".join(str(int(x)) for x in row))
        return
    print("CORRUPT")

if __name__=="__main__":
    solve()
