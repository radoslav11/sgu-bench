import sys
sys.setrecursionlimit(10**7)
def solve():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    n, m, K = map(int, (next(it), next(it), next(it)))
    a = [[int(next(it)) for _ in range(m)] for __ in range(n)]

    # Quick exit if K == 0
    if K == 0:
        print("Oil : 0")
        return

    # Build 2D prefix sums
    ps = [[0]*m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            ps[i][j] = a[i][j]
            if i>0: ps[i][j] += ps[i-1][j]
            if j>0: ps[i][j] += ps[i][j-1]
            if i>0 and j>0: ps[i][j] -= ps[i-1][j-1]

    def rectsum(x1,y1,x2,y2):
        """Sum of a[x1..x2][y1..y2] in O(1)."""
        res = ps[x2][y2]
        if x1>0: res -= ps[x1-1][y2]
        if y1>0: res -= ps[x2][y1-1]
        if x1>0 and y1>0: res += ps[x1-1][y1-1]
        return res

    from functools import lru_cache
    INF = 10**12

    @lru_cache(None)
    def dp(row, L, R, rem, mask):
        """
        Max oil from rows row..n-1,
        if previous row used segment [L,R],
        rem cells still needed,
        mask bits: 1=left locked, 2=right locked.
        """
        # If we've processed all rows
        if row==n:
            return 0 if rem==0 else -INF
        # If no more cells to pick
        if rem==0:
            return 0
        best = -INF
        canL = not(mask & 1)
        canR = not(mask & 2)
        # Try all segments [l2..r2] in this row
        for l2 in range(m):
            for r2 in range(l2, m):
                length = r2 - l2 + 1
                if length>rem: continue
                # Must overlap with [L,R]
                if min(R,r2) < max(L,l2): continue
                # Check locks
                if (not canL) and l2 < L: continue
                if (not canR) and r2 > R: continue
                # Update mask
                nm = mask
                if l2 > L: nm |= 1
                if r2 < R: nm |= 2
                s = rectsum(row, l2, row, r2)
                val = s + dp(row+1, l2, r2, rem-length, nm)
                if val>best:
                    best = val
        return best

    # Find best starting row and segment
    ansVal = -INF
    start = None
    for row in range(n):
        for L in range(m):
            for R in range(L, m):
                length = R-L+1
                if length>K: continue
                s = rectsum(row, L, row, R)
                val = s + dp(row+1, L, R, K-length, 0)
                if val>ansVal:
                    ansVal = val
                    start = (row, L, R)

    # Output maximum
    print("Oil : {}".format(ansVal))

    # Reconstruct chosen cells
    (row, L, R) = start
    rem = K
    mask = 0
    cells = []

    # Take initial segment
    for c in range(L, R+1):
        cells.append((row+1, c+1))
    rem -= (R-L+1)
    ansVal -= rectsum(row, L, row, R)
    row += 1

    # Walk forward until rem==0
    while rem>0:
        target = ansVal
        canL = not(mask & 1)
        canR = not(mask & 2)
        found = None
        for l2 in range(m):
            for r2 in range(l2, m):
                length = r2-l2+1
                if length>rem: continue
                if min(R,r2) < max(L,l2): continue
                if (not canL) and l2<L: continue
                if (not canR) and r2>R: continue
                nm = mask
                if l2>L: nm |= 1
                if r2<R: nm |= 2
                s = rectsum(row, l2, row, r2)
                if s + dp(row+1, l2, r2, rem-length, nm) == target:
                    found = (l2, r2, nm)
                    break
            if found: break
        l2, r2, mask = found
        # Add these cells
        for c in range(l2, r2+1):
            cells.append((row+1, c+1))
        rem -= (r2-l2+1)
        ansVal -= rectsum(row, l2, row, r2)
        L, R = l2, r2
        row += 1

    # Sort and print
    cells.sort()
    for x,y in cells:
        print(x, y)

if __name__ == "__main__":
    solve()
