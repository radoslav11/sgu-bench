import sys
import threading
def main():
    sys.setrecursionlimit(10**7)
    data = sys.stdin.read().split()
    it = iter(data)
    try:
        n = int(next(it))
    except StopIteration:
        return
    # Read matrices
    up    = [[int(next(it)) for _ in range(n)] for _ in range(n)]
    left  = [[int(next(it)) for _ in range(n)] for _ in range(n)]
    # 1) Feasibility check
    for i in range(n):
        for j in range(n):
            if left[i][j] > j or up[i][j] > i:
                print(0)
                return

    # Fenwick tree supporting point updates and prefix sums
    class Fenw:
        def __init__(self, n):
            self.n = n
            self.f = [0]*(n+1)
        def update(self, i, v):
            # add v at index i (1-based)
            while i <= self.n:
                self.f[i] += v
                i += i & -i
        def query(self, i):
            # sum f[1..i]
            s = 0
            while i>0:
                s += self.f[i]
                i -= i & -i
            return s
        def find_kth(self, k):
            # find smallest i with prefix sum ≥ k
            idx = 0
            bit = 1<<(self.n.bit_length())
            while bit>0:
                nxt = idx+bit
                if nxt<=self.n and self.f[nxt]<k:
                    k -= self.f[nxt]
                    idx = nxt
                bit >>= 1
            return idx+1

    # Build adjacency list for DAG of size n*n
    N = n*n
    adj = [[] for _ in range(N)]
    indeg = [0]*N

    # Helper to add chain edges given a Lehmer code row/col
    def process_block(code, is_row, idx):
        # code: list of length n of inversion counts
        # is_row: True if it's a row, idx is row-index, else column
        fenw = Fenw(n)
        for i in range(1, n+1):
            fenw.update(i, 1)
        vals = [0]*n
        # decode from right to left
        for pos in range(n-1, -1, -1):
            cnt = code[pos]
            place = fenw.find_kth(cnt+1)
            node = (idx*n + pos) if is_row else (pos*n + idx)
            vals[place-1] = node
            fenw.update(place, -1)
        # vals[0]..vals[n-1] are nodes in DESCENDING A-value order: the
        # decode hands slot 1 to the cell with 0 larger elements before
        # it, i.e. the largest. Edge u->v must mean A[u] < A[v].
        for k in range(n-1):
            u = vals[k + 1]
            v = vals[k]
            adj[u].append(v)
            indeg[v] += 1

    # 2) rows
    for i in range(n):
        process_block(left[i], True, i)
    # 3) columns
    for j in range(n):
        col_code = [up[i][j] for i in range(n)]
        process_block(col_code, False, j)

    # 4) Topological sort (Kahn)
    from collections import deque
    dq = deque(u for u in range(N) if indeg[u]==0)
    ans = [0]*N
    cur = 1
    while dq:
        u = dq.popleft()
        ans[u] = cur
        cur += 1
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v]==0:
                dq.append(v)
    if cur != N+1:
        print(0)
        return

    # 5) Print matrix
    out = []
    for i in range(n):
        row = ans[i*n:(i+1)*n]
        out.append(" ".join(map(str,row)))
    print("\n".join(out))

if __name__ == "__main__":
    main()
